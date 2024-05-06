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
	auto metadata = XML.appendChild("IMAGE");

	// Set the FileName
	metadata.appendChild("FILENAME").set(filename);

	// Adds tags section
	auto xmlTags = metadata.appendChild("TAGS");

	ofColor color = calculateAverageColorInFrame(img.getPixels());
	int luminance = calculateLuminance(color);

	// add Luminance
	metadata.appendChild("LUMINANCE").set(luminance);

	// add color
	auto colorSection = metadata.appendChild("COLOR");
	colorSection.appendChild("RED").set(color.r);
	colorSection.appendChild("GREEN").set(color.g);
	colorSection.appendChild("BLUE").set(color.b);

	// add Number of faces
	metadata.appendChild("NUM_FACES");

	metadata.appendChild("EDGE_DISTRIBUTION");

	metadata.appendChild("TEXTURE_CHARACTERISTICS");

	metadata.appendChild("OBJECT_OCCURRENCES");
}

void Metadata::createVideoFile(string filename, ofXml& XML) {

	ofVideoPlayer video;
	video.load("videos/" + filename);

	auto metadata = XML.appendChild("VIDEO");

	// Set the FileName
	metadata.appendChild("FILENAME").set(filename);

	// Adds tags section
	auto xmlTags = metadata.appendChild("TAGS");

	ofPixels& pixels = pixelsFromFirstFrame(video);
	ofColor color = calculateAverageColorInFrame(pixels);
	int luminance = calculateLuminance(color);

	// add Luminance
	metadata.appendChild("LUMINANCE").set(luminance);

	// add color
	auto colorSection = metadata.appendChild("COLOR");
	colorSection.appendChild("RED").set(color.r);
	colorSection.appendChild("GREEN").set(color.g);
	colorSection.appendChild("BLUE").set(color.b);

	// add Number of faces
	metadata.appendChild("NUM_FACES");

	metadata.appendChild("EDGE_DISTRIBUTION");

	metadata.appendChild("TEXTURE_CHARACTERISTICS");

	metadata.appendChild("OBJECT_OCCURRENCES");
}


ofColor Metadata::calculateAverageColorInFrame(ofPixels& pixels) {

	int nPixels = pixels.getWidth() * pixels.getHeight();

	int r = 0, g = 0, b = 0;
	for (int i = 0; i < nPixels; i++) {
		int index = i * 3;
		r += pixels[index];
		g += pixels[index + 1];
		b += pixels[index + 2];
	}

	r /= nPixels;
	g /= nPixels;
	b /= nPixels;

	return ofColor(r, g, b);
}

ofPixels& Metadata::pixelsFromFirstFrame(ofVideoPlayer& video) {
	video.play();

	ofSleepMillis(500);

	video.setPaused(true);

	video.update();

	while (!video.isFrameNew())
	{
		video.nextFrame();
		video.update();
	}

	return video.getPixels();
}


int Metadata::calculateLuminance(ofColor color) {
	return 0.2125 * color.r + 0.7154 * color.g + 0.0721 * color.b;
}