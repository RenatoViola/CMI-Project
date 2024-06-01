#include "Metadata.h"
#include "VideoMedia.h"

//--------------------------------------------------------------
void Metadata::load(string filename, ofXml& XML, bool isImage) {

	const string PATH = (isImage ? "imagesXML/" : "videosXML/") + filename + ".xml";

	if (!XML.load(PATH)) {
		ofLogError() << "Couldn't load file: " << filename << "; " << "Will create one.";
		XML.clear();
		if (isImage)
		{
			ofImage img;
			img.load("images/" + filename);
			vector<ofPixels> frames{ img.getPixels() };

			processFileMetadata(filename, frames, XML.appendChild("IMAGE"));
		}
		else
		{
			ofVideoPlayer video;
			video.load("videos/" + filename);
			vector<ofPixels> frames = VideoMedia::extractFrames(video, 10);

			processFileMetadata(filename, frames, XML.appendChild("VIDEO"));
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

/*
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
*/
