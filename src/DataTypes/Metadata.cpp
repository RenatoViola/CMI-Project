#include "Metadata.h"
#include "VideoMedia.h"
#include <opencv2/features2d.hpp>
#include <constants_c.h>


//--------------------------------------------------------------
void Metadata::load(string filename, ofXml& XML, bool isImage) {

	const string PATH = (isImage ? "imagesXML/" : "videosXML/") + filename + ".xml";

	if (!XML.load(PATH)) {
		ofLogError() << "Couldn't load file: " << filename << "; " << "Will create one.";
		XML.clear();
		ofXml file = XML.appendChild("FILE");
		vector<ofPixels> frames;

		if (isImage)
		{
			ofImage img;
			img.load("images/" + filename);
			frames = { img.getPixels() };
			file.appendChild("TYPE").set("IMAGE");
		}
		else
		{
			ofVideoPlayer video;
			video.load("videos/" + filename);
			frames = VideoMedia::extractFrames(video, 10);
			file.appendChild("TYPE").set("VIDEO");
		}
		processFileMetadata(filename, frames, file);
		save(filename, XML, isImage);
	}
}

//--------------------------------------------------------------
void Metadata::save(string filename, ofXml& XML, bool isImage) {
	const string PATH = (isImage ? "imagesXML/" : "videosXML/") + filename + ".xml";

	if (!XML.save(PATH)) {
		ofLogError() << "Couldn't save file:" << filename << ";";
	}
}

//--------------------------------------------------------------
set<string> Metadata::getTags(ofXml& XML) {
	set<string> tags;

	auto xmlTags = XML.find("//TAGS/TAG");

	if (xmlTags.empty())
	{
		throw std::runtime_error("There are no tags for this file.");
	}

	for (auto& tag : xmlTags) {
		tags.insert(tag.getValue());
	}

	return tags;
}

//--------------------------------------------------------------
void Metadata::processFileMetadata(string filename, vector<ofPixels>& frames, ofXml& XML) {

	XML.appendChild("FILENAME").set(filename);
	auto xmlTags = XML.appendChild("TAGS");

	calculateAverageColorAndLuminance(frames, XML);

	int num_faces = numberOfFaces(frames);
	XML.appendChild("NUM_FACES").set(num_faces);

	ofPixels& firstFrame = frames.at(0);
	detectEdges(firstFrame, XML.appendChild("EDGE_DISTRIBUTION"));
	detectTextureCharacteristics(firstFrame, XML.appendChild("TEXTURE_CHARACTERISTICS"));
	
	XML.appendChild("OBJECT_OCCURRENCES");
}


void Metadata::calculateAverageColorAndLuminance(vector<ofPixels>& frames, ofXml& XML) {

	long long r = 0, g = 0, b = 0;
	float totalLuminance = 0.0;
	
	int count = 0;

	for (ofPixels& f : frames)
	{
		ofColor colorInFrame;
		float luminanceInFrame;
		calculateAverageColorAndLuminanceInFrame(f, colorInFrame, &luminanceInFrame);

		r += colorInFrame.r;
		g += colorInFrame.g;
		b += colorInFrame.b;
		totalLuminance += luminanceInFrame;
	}

	int nFrames = frames.size();

	float luminance = totalLuminance / nFrames;
	XML.appendChild("LUMINANCE").set(luminance);

	ofColor color = ofColor(r / nFrames, g / nFrames, b / nFrames);

	auto colorSection = XML.appendChild("COLOR");
	colorSection.appendChild("RED").set(color.r);
	colorSection.appendChild("GREEN").set(color.g);
	colorSection.appendChild("BLUE").set(color.b);
}

void Metadata::calculateAverageColorAndLuminanceInFrame(ofPixels& pixels, ofColor& color, float* luminance) {

	int nPixels = pixels.getWidth() * pixels.getHeight();
	long long r = 0, g = 0, b = 0;
	float totalLuminance = 0.0;

	for (size_t i = 0; i < nPixels; i++) {
		size_t index = i * 3;
		unsigned char R = pixels[index];
		unsigned char G = pixels[index + 1];
		unsigned char B = pixels[index + 2];

		r += R;
		g += G;
		b += B;
		totalLuminance += 0.2125 * R + 0.7154 * G + 0.0721 * B;
	}

	color.r = r / nPixels;
	color.g = g / nPixels;
	color.b = b / nPixels;

	*luminance = totalLuminance / nPixels;
}

int Metadata::numberOfFaces(vector<ofPixels>& frames) {
	ofxCvHaarFinder finder;
	finder.setup("haarcascade_frontalface_alt_tree.xml");

	int nFaces = 0;
	ofxCvColorImage colorImg;
	ofxCvGrayscaleImage grayImg;
	int width = frames.at(0).getWidth(), height = frames.at(0).getHeight();

	colorImg.allocate(width, height);
	grayImg.allocate(width, height);

	for (ofPixels& f : frames)
	{
		colorImg.setFromPixels(f);
		grayImg = colorImg;
		finder.findHaarObjects(grayImg);

		nFaces += finder.blobs.size();
	}

	return nFaces / frames.size();
}

void Metadata::detectEdges(ofPixels& pixels, ofXml& XML) {

	Mat src(pixels.getWidth(), pixels.getHeight(), CV_8UC(pixels.getNumChannels()), pixels.getData());
	cvtColor(src, src, COLOR_RGB2GRAY);

	Mat dst, kernel;

	kernel = (Mat_<char>(2, 2) << 1, -1, 1, -1);
	cv::filter2D(src, dst, src.depth(), kernel);
	calculateStats("VERTICAL", dst, XML, true);

	kernel = (Mat_<char>(2, 2) << 1, 1, -1, -1);
	cv::filter2D(src, dst, src.depth(), kernel);
	calculateStats("HORIZONTAL", dst, XML, true);

	kernel = (Mat_<char>(2, 2) << sqrt(2), 0, 0, -sqrt(2));
	cv::filter2D(src, dst, src.depth(), kernel);
	calculateStats("DEGREES_45", dst, XML, true);

	kernel = (Mat_<char>(2, 2) << 0, sqrt(2), -sqrt(2), 0);
	cv::filter2D(src, dst, src.depth(), kernel);
	calculateStats("DEGREES_135", dst, XML, true);

	kernel = (Mat_<char>(2, 2) << 2, -2, -2, 2);
	cv::filter2D(src, dst, src.depth(), kernel);
	calculateStats("NON_DIRECTIONAL", dst, XML, true);
}


void Metadata::detectTextureCharacteristics(ofPixels& pixels, ofXml& XML) {

	Mat src(pixels.getWidth(), pixels.getHeight(), CV_8UC(pixels.getNumChannels()), pixels.getData());
	cvtColor(src, src, COLOR_RGB2GRAY);

	Mat dst, kernel;

	Size ksize = Size(5, 5);
	double lambd = 50.0;   
	double gamma = 0.5;

	int kernel_id = 0;

	for (int theta = 30; theta <= 180; theta += 30)
	{
		for (int sigma = 15; sigma <= 60; sigma += 15) {
			kernel = getGaborKernel(ksize, sigma, theta, lambd, gamma);
			cv::filter2D(src, dst, src.depth(), kernel);
			string kernel_name = "GABOR_KERNEL_" + to_string(++kernel_id);
			calculateStats(kernel_name, dst, XML, false);
		}
	}
}

void Metadata::calculateStats(const string& filterName, Mat& filteredMat, ofXml& XML, bool thresholdOn) {

	int numEdges;

	if (thresholdOn)
	{
		Mat binaryMat;
		threshold(filteredMat, binaryMat, 0, 255, THRESH_BINARY | THRESH_OTSU);
		numEdges = countNonZero(binaryMat);
	}
	else
	{
		numEdges = countNonZero(filteredMat);
	}

	Scalar mean, deviation;
	meanStdDev(filteredMat, mean, deviation);

	auto filterSection = XML.appendChild(filterName);
	filterSection.appendChild("EDGES").set(numEdges);
	filterSection.appendChild("MEAN").set(mean[0]);
	filterSection.appendChild("STANDARD_DEVIATION").set(deviation[0]);
}


vector<string> Metadata::filesWithObject(ofPixels& pixels, vector<string>& image_paths, vector<string> video_paths) {

	vector<pair<string, int>> vec;

	Mat img1(pixels.getWidth(), pixels.getHeight(), CV_8UC(pixels.getNumChannels()), pixels.getData());
	if (img1.channels() != 1) {
		cvtColor(img1, img1, COLOR_RGB2GRAY);
	}
	
	Mat desc1;
	vector<KeyPoint> kpts1;

	Ptr<Feature2D> detector = SIFT::create();
	detector->detectAndCompute(img1, Mat(), kpts1, desc1);
	
	for (string path : image_paths)
	{
		ofImage image2;
		image2.load(path);
		int occurrences = countOccurrencesInFrame(image2.getPixels(), desc1, kpts1);
		if (occurrences > 0)
		{
			vec.emplace_back(path, occurrences);
		}
	}

	for (string path : video_paths)
	{
		ofVideoPlayer video;
		video.load(path);
		ofPixels frame = VideoMedia::extractFirstFrame(video);
		int occurrences = countOccurrencesInFrame(frame, desc1, kpts1);
		if (occurrences > 0)
		{
			vec.emplace_back(path, occurrences);
		}
	}

	sort(vec.begin(), vec.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
		return a.second > b.second;
	});

	if (vec.size() > 8) {
		vec.resize(8);
	}

	vector<std::string> filenames;
	filenames.reserve(vec.size());

	for (const auto& pair : vec) {
		filenames.push_back(pair.first);
	}

	for (const auto& pair : vec) {
		ofLogError() << "PAIR " << pair.first << ": " << pair.second << std::endl;
	}

	return filenames;
}


int Metadata::countOccurrencesInFrame(ofPixels& pixels, Mat& desc1, vector<KeyPoint>& kpts1) {

	// Might need to tweak these values further
	const double kDistanceCoef = 2.0;
	const int kMaxMatchingSize = 50;

	Mat img2(pixels.getWidth(), pixels.getHeight(), CV_8UC(pixels.getNumChannels()), pixels.getData());
	if (img2.channels() != 1) {
		cvtColor(img2, img2, COLOR_RGB2GRAY);
	}

	Mat desc2;
	vector<KeyPoint> kpts2;

	Ptr<Feature2D> detector = SIFT::create();
	detector->detectAndCompute(img2, Mat(), kpts2, desc2);

	BFMatcher desc_matcher(cv::NORM_L2, true);
	vector<DMatch> matches;
//	desc_matcher.match(desc1, desc2, matches, Mat());  // DEFAULT

	//______________________________________________________________KNN
	vector< vector<DMatch> > vmatches;
	desc_matcher.knnMatch(desc1, desc2, vmatches, 1);
	for (int i = 0; i < static_cast<int>(vmatches.size()); ++i) {
		if (!vmatches[i].size()) {
			continue;
		}
		matches.push_back(vmatches[i][0]);
	}
	//______________________________________________________________
	std::sort(matches.begin(), matches.end());
	while (!matches.empty() && matches.front().distance * kDistanceCoef < matches.back().distance) {
		matches.pop_back();
	}
	while (matches.size() > kMaxMatchingSize) {
		matches.pop_back();
	}

	vector<char> match_mask(matches.size(), 1);
	if (static_cast<int>(match_mask.size()) < 3) {
		return 0;
	}

	vector<Point2f> pts1, pts2;
	for (int i = 0; i < static_cast<int>(matches.size()); ++i) {
		pts1.push_back(kpts1[matches[i].queryIdx].pt);
		pts2.push_back(kpts2[matches[i].trainIdx].pt);
	}
	
	findHomography(pts1, pts2, cv::RANSAC, 4, match_mask);

	return countNonZero(match_mask);
}

FileMetadata Metadata::parseMetadata(const string& filename) {
	FileMetadata metadata;
	ofXml xml;

	if (!xml.load(filename)) {
		cerr << "Error loading file: " << filename << endl;
		return metadata;
	}

	// Parse file type
	metadata.path = xml.getAttribute("TYPE").getValue();

	// Parse filename
	metadata.path = xml.getAttribute("FILENAME").getValue();

	// Parse tags
	metadata.tags = getTags(xml);

	// Parse luminance
	metadata.luminance = xml.getAttribute("LUMINANCE").getDoubleValue();

	// Parse color
	auto color = xml.findFirst("COLOR");
	metadata.red = color.getChild("RED").getIntValue();
	metadata.green = color.getChild("GREEN").getIntValue();
	metadata.blue = color.getChild("BLUE").getIntValue();

	// Parse number of faces
	metadata.numFaces = xml.getAttribute("NUM_FACES").getIntValue();

	// Parse edge distribution
	vector<string> edgeTags = { "VERTICAL", "HORIZONTAL", "DEGREES_45", "DEGREES_135", "NON_DIRECTIONAL" };
	for (size_t i = 0; i < edgeTags.size(); i++) {
		auto filter = xml.findFirst("EDGE_DISTRIBUTION/" + edgeTags[i]);
		int edges = filter.getChild("EDGES").getIntValue();
		double mean = filter.getChild("MEAN").getDoubleValue();
		double stdDev = filter.getChild("STANDARD_DEVIATION").getDoubleValue();
		metadata.edgeDistribution[i] = make_tuple(edges, mean, stdDev);
	}

	// Parse texture characteristics
	for (int i = 0; i < 24; i++) {
		string tag = "GABOR_KERNEL_" + to_string(i + 1);
		auto gabor = xml.findFirst("TEXTURE_CHARACTERISTICS/" + tag);
		int edges = gabor.getChild("EDGES").getIntValue();
		double mean = gabor.getChild("MEAN").getDoubleValue();
		double stdDev = gabor.getChild("STANDARD_DEVIATION").getDoubleValue();
		metadata.textureCharacteristics[i] = make_tuple(edges, mean, stdDev);
	}
	
	return metadata;
}


double Metadata::calculateSimilarity(const FileMetadata& file1, const FileMetadata& file2) {
	
	double similarity = 0.0;

	set<string> tags1 = file1.tags, tags2 = file2.tags;

	// Create an empty set to store the intersection of set1 and set2
	set<string> intersection;

	// Find the intersection of the two sets
	std::set_intersection(tags1.begin(), tags1.end(),
		tags2.begin(), tags2.end(),
		std::inserter(intersection, intersection.begin()));

	// We use a high weight for the tag similarity score
	double tagSimilarity = intersection.size();

	// Compare luminance (normalize the difference)
	similarity += 1.0 - abs(file1.luminance - file2.luminance) / 255.0;

	// Compare color (Euclidean distance normalized)
	double colorDist = sqrt(pow(file1.red - file2.red, 2) +
		pow(file1.green - file2.green, 2) +
		pow(file1.blue - file2.blue, 2));
	similarity += 1.0 - colorDist / sqrt(3 * pow(255, 2));

	// Compare number of faces (normalized difference)
	similarity += 1.0 - abs(file1.numFaces - file2.numFaces) / max(file1.numFaces, file2.numFaces + 1);

	// Compare edge distribution (Euclidean distance normalized)
	double edgeDist = 0.0;
	for (size_t i = 0; i < file1.edgeDistribution.size(); i++) {
		edgeDist += pow(get<0>(file1.edgeDistribution[i]) - get<0>(file2.edgeDistribution[i]), 2);
		edgeDist += pow(get<1>(file1.edgeDistribution[i]) - get<1>(file2.edgeDistribution[i]), 2);
		edgeDist += pow(get<2>(file1.edgeDistribution[i]) - get<2>(file2.edgeDistribution[i]), 2);
	}
	edgeDist = sqrt(edgeDist);
	similarity += 1.0 - edgeDist / sqrt(file1.edgeDistribution.size() * 3 * pow(100, 2)); // Assuming normalization factor 100

	// Compare texture characteristics (Euclidean distance normalized)
	double textureDist = 0.0;
	for (size_t i = 0; i < file1.textureCharacteristics.size(); i++) {
		textureDist += pow(get<0>(file1.textureCharacteristics[i]) - get<0>(file2.textureCharacteristics[i]), 2);
		textureDist += pow(get<1>(file1.textureCharacteristics[i]) - get<1>(file2.textureCharacteristics[i]), 2);
		textureDist += pow(get<2>(file1.textureCharacteristics[i]) - get<2>(file2.textureCharacteristics[i]), 2);
	}
	textureDist = sqrt(textureDist);
	similarity += 1.0 - textureDist / sqrt(file1.textureCharacteristics.size() * 3 * pow(100, 2)); // Assuming normalization factor 100

	// Normalize similarity score to range [0, 1]
	similarity /= 5.0;

	// Combine tag similarity and attribute similarity
	double combinedSimilarity = tagSimilarity * 1000 + similarity; // Using a large weight for tags to prioritize them

	return combinedSimilarity;
}


vector<string> Metadata::findRelatedFiles(string filepath, vector<string>& image_paths, vector<string> video_paths) {
	
	vector<pair<string, double>> vec;

	FileMetadata f1 = parseMetadata(filepath);
	FileMetadata f2;
	double similarity;

	for (string path : image_paths)
	{
		f2 = parseMetadata(path);
		similarity = calculateSimilarity(f1, f2);
		vec.emplace_back(path, similarity);
	}

	for (string path : video_paths)
	{
		f2 = parseMetadata(path);
		similarity = calculateSimilarity(f1, f2);
		vec.emplace_back(path, similarity);
	}

	sort(vec.begin(), vec.end(), [](const pair<string, double>& a, const pair<string, double>& b) {
		return a.second > b.second;
	});

	if (vec.size() > 8) {
		vec.resize(8);
	}

	vector<std::string> filenames;
	filenames.reserve(vec.size());

	for (const auto& pair : vec) {
		filenames.push_back(pair.first);
	}

	for (const auto& pair : vec) {
		ofLogError() << "PAIR " << pair.first << ": " << pair.second << std::endl;
	}

	return filenames;
}