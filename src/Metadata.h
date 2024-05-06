#pragma once

#include "ofMain.h"


class Metadata
{
public:
	static void load(string filename, ofXml& XML, bool isImage);
	static void save(string filename, ofXml& XML, bool isImage);
	static vector<string> getTags(ofXml& XML);

private:
	static void createImageFile(string fileName, ofXml& XML);
	static void createVideoFile(string fileName, ofXml& XML);
	static ofColor calculateAverageColorInFrame(ofPixels& pixels);
	static ofPixels& pixelsFromFirstFrame(ofVideoPlayer& video);
	static int calculateLuminance(ofColor color);
	static int numberOfFaces(ofPixels& pixels);
};

