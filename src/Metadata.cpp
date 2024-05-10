#include "Metadata.h"


//--------------------------------------------------------------
void Metadata::load(string filename, ofXml& XML, bool isImage) {

	const string PATH = (isImage ? "imagesXML/" : "videosXML/") + filename + ".xml";

	if (!XML.load(PATH)) {
		ofLogError() << "Couldn't load file: " << filename << "; " << "Will create one.";
		XML.clear();
		if (isImage)
		{
			createImageFile(filename, XML);
		}
		else
		{
			createVideoFile(filename, XML);
		}
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
vector<string> Metadata::getTags(ofXml& XML) {
	vector<string> tags;

	auto xmlTags = XML.find("//TAGS/TAG");

	if (xmlTags.empty())
	{
		throw new exception("There are no tags for this file.");
	}

	for (auto& tag : xmlTags) {
		tags.push_back(tag.getValue());
	}

	return tags;
}

//--------------------------------------------------------------
void Metadata::createImageFile(string filename, ofXml& XML) {

	ofImage img;
	img.load("images/" + filename);
	ofPixels& firstFrame = img.getPixels();
	vector<ofPixels> frames{ firstFrame };

	auto metadata = XML.appendChild("IMAGE");

	// Set the FileName
	metadata.appendChild("FILENAME").set(filename);

	// Adds tags section
	auto xmlTags = metadata.appendChild("TAGS");

	ofColor color;
	float luminance;
	calculateAverageColorAndLuminance(frames, color, &luminance);

	// add Luminance
	metadata.appendChild("LUMINANCE").set(luminance);

	// add color
	auto colorSection = metadata.appendChild("COLOR");
	colorSection.appendChild("RED").set(color.r);
	colorSection.appendChild("GREEN").set(color.g);
	colorSection.appendChild("BLUE").set(color.b);

	// add Number of faces
	int num_faces = numberOfFaces(frames);
	metadata.appendChild("NUM_FACES").set(num_faces);

	auto edgeSection = metadata.appendChild("EDGE_DISTRIBUTION");
	detectEdges(firstFrame, edgeSection);

	metadata.appendChild("TEXTURE_CHARACTERISTICS");

	metadata.appendChild("OBJECT_OCCURRENCES");
}

void Metadata::createVideoFile(string filename, ofXml& XML) {

	ofVideoPlayer video;
	video.load("videos/" + filename);
	vector<ofPixels> frames = extractFrames(video, 4);
	ofPixels& firstFrame = frames.at(0);

	auto metadata = XML.appendChild("VIDEO");

	// Set the FileName
	metadata.appendChild("FILENAME").set(filename);

	// Adds tags section
	auto xmlTags = metadata.appendChild("TAGS");

	ofColor color;
	float luminance;
	calculateAverageColorAndLuminance(frames, color, &luminance);

	// add Luminance
	metadata.appendChild("LUMINANCE").set(luminance);

	// add color
	auto colorSection = metadata.appendChild("COLOR");
	colorSection.appendChild("RED").set(color.r);
	colorSection.appendChild("GREEN").set(color.g);
	colorSection.appendChild("BLUE").set(color.b);

	// add Number of faces
	int num_faces = numberOfFaces(frames);
	metadata.appendChild("NUM_FACES").set(num_faces);

	auto edgeSection = metadata.appendChild("EDGE_DISTRIBUTION");
	detectEdges(firstFrame, edgeSection);

	metadata.appendChild("TEXTURE_CHARACTERISTICS");

	metadata.appendChild("OBJECT_OCCURRENCES");
}


void Metadata::calculateAverageColorAndLuminance(vector<ofPixels> frames, ofColor& color, float* luminance) {

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

	color.r = r / nFrames;
	color.g = g / nFrames;
	color.b = b / nFrames;

	*luminance = totalLuminance / nFrames;
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


int Metadata::numberOfFaces(vector<ofPixels> frames) {
	ofxCvHaarFinder finder;
	finder.setup("haarcascade_frontalface_default.xml");

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

	Mat input(pixels.getWidth(), pixels.getHeight(), CV_8UC(pixels.getNumChannels()), pixels.getData());
	cvtColor(input, input, COLOR_RGB2GRAY);

	Mat vertMat, horMat, d45Mat, d135Mat, ndMat, kernel;

	kernel = (Mat_<char>(2, 2) << 1, -1, 1, -1);
	cv::filter2D(input, vertMat, input.depth(), kernel);
	calculateEdgesAndStats("VERTICAL", vertMat, XML);

	kernel = (Mat_<char>(2, 2) << 1, 1, -1, -1);
	cv::filter2D(input, horMat, input.depth(), kernel);
	calculateEdgesAndStats("HORIZONTAL", horMat, XML);

	kernel = (Mat_<char>(2, 2) << sqrt(2), 0, 0, -sqrt(2));
	cv::filter2D(input, d45Mat, input.depth(), kernel);
	calculateEdgesAndStats("DEGREES_45", d45Mat, XML);

	kernel = (Mat_<char>(2, 2) << 0, sqrt(2), -sqrt(2), 0);
	cv::filter2D(input, d135Mat, input.depth(), kernel);
	calculateEdgesAndStats("DEGREES_135", d135Mat, XML);

	kernel = (Mat_<char>(2, 2) << 2, -2, -2, 2);
	cv::filter2D(input, ndMat, input.depth(), kernel);
	calculateEdgesAndStats("NON_DIRECTIONAL", ndMat, XML);
}

void Metadata::calculateEdgesAndStats(const string& filterName, Mat& filteredMat, ofXml& XML) {
	Mat binaryMat;
	threshold(filteredMat, binaryMat, 0, 255, THRESH_BINARY | THRESH_OTSU);

	int numEdges = countNonZero(binaryMat);

	Scalar mean, deviation;
	meanStdDev(filteredMat, mean, deviation);

	auto filterSection = XML.appendChild(filterName);
	filterSection.appendChild("NUM_EDGES").set(numEdges);
	filterSection.appendChild("MEAN").set(mean[0]);
	filterSection.appendChild("STANDARD_DEVIATION").set(deviation[0]);
}

vector<ofPixels> Metadata::extractFrames(ofVideoPlayer& videoPlayer, int skip) {
	vector<ofPixels> frames;

	videoPlayer.update();
	videoPlayer.play();

	ofSleepMillis(500);

	videoPlayer.setPaused(true);
	videoPlayer.update();

	int currentFrame = 0;
	int totalFrames = videoPlayer.getTotalNumFrames();

	while (currentFrame < totalFrames) {
		if (currentFrame % (skip + 1) == 0) {
			while (!videoPlayer.isFrameNew())
			{
				videoPlayer.update();
			}
			frames.push_back(videoPlayer.getPixels());
		}
		videoPlayer.nextFrame();
		currentFrame++;
	}
	videoPlayer.stop();

	return frames;
}