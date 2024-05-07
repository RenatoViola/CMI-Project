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
	vector<ofPixels> frames{ img.getPixels() };

	auto metadata = XML.appendChild("IMAGE");

	// Set the FileName
	metadata.appendChild("FILENAME").set(filename);

	// Adds tags section
	auto xmlTags = metadata.appendChild("TAGS");

	ofColor color = calculateAverageColor(frames);
	int luminance = calculateLuminance(color);

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

	metadata.appendChild("EDGE_DISTRIBUTION");

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

	ofColor color = calculateAverageColor(frames);
	
	int luminance = calculateLuminance(color);

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

	metadata.appendChild("EDGE_DISTRIBUTION");

	metadata.appendChild("TEXTURE_CHARACTERISTICS");

	metadata.appendChild("OBJECT_OCCURRENCES");
}


vector<ofPixels> Metadata::extractFrames(ofVideoPlayer& videoPlayer, int skip) {
	vector<ofPixels> frames;

	videoPlayer.setPaused(true);

	for (int i = 0; i < videoPlayer.getTotalNumFrames(); i += skip + 1) {
		videoPlayer.setFrame(i);
		videoPlayer.update();

		frames.push_back(videoPlayer.getPixels());
	}

	return frames;
}

ofColor Metadata::calculateAverageColor(vector<ofPixels> frames) {

	long long r = 0, g = 0, b = 0;

	for (ofPixels& f : frames)
	{
		ofColor color = calculateAverageColorInFrame(f);
		r += color.r;
		g += color.g;
		b += color.b;
	}

	int nFrames = frames.size();

	r /= nFrames;
	g /= nFrames;
	b /= nFrames;

	return ofColor(r, g, b);
}

ofColor Metadata::calculateAverageColorInFrame(ofPixels& pixels) {

	int nPixels = pixels.getWidth() * pixels.getHeight();

	long long r = 0, g = 0, b = 0;

	for (size_t i = 0; i < nPixels; i++) {
		size_t index = i * 3;
		r += pixels[index];
		g += pixels[index + 1];
		b += pixels[index + 2];
	}

	r /= nPixels;
	g /= nPixels;
	b /= nPixels;

	return ofColor(r, g, b);
}


int Metadata::calculateLuminance(ofColor color) {
	return 0.2125 * color.r + 0.7154 * color.g + 0.0721 * color.b;
}

int Metadata::numberOfFaces(vector<ofPixels> frames) {
	ofxCvHaarFinder finder;
	finder.setup("haarcascade_frontalface_default.xml");

	int nFaces = 0;

	for (ofPixels& f : frames)
	{
		nFaces += numberOfFacesInFrame(f, finder);
	}

	return nFaces / frames.size();
}

int Metadata::numberOfFacesInFrame(ofPixels& pixels, ofxCvHaarFinder& finder) {

	ofxCvColorImage colorImg;
	ofxCvGrayscaleImage grayImg;
	int width = pixels.getWidth(), height = pixels.getHeight();

	colorImg.allocate(width, height);
	grayImg.allocate(width, height);

	colorImg.setFromPixels(pixels);
	grayImg = colorImg;
	finder.findHaarObjects(grayImg);

	return finder.blobs.size();
}