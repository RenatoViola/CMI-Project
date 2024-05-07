#pragma once

#include "ofMain.h"
#include "ofxCvHaarFinder.h"

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
	static ofColor Metadata::calculateAverageColor(vector<ofPixels> frames);
	static vector<ofPixels> extractFrames(ofVideoPlayer& videoPlayer, int skip);
	static int calculateLuminance(ofColor color);
	static int numberOfFacesInFrame(ofPixels& pixels, ofxCvHaarFinder& finder);
	static int numberOfFaces(vector<ofPixels> frames);
};

