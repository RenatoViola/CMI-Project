#pragma once

#include "ofMain.h"
#include "ofxCvHaarFinder.h"
using namespace cv;

class Metadata
{
public:
	static void load(string filename, ofXml& XML, bool isImage);
	static void save(string filename, ofXml& XML, bool isImage);
	static vector<string> getTags(ofXml& XML);

private:
	static void processFileMetadata(string fileName, vector<ofPixels>& frames, ofXml& XML);
	static void calculateAverageColorAndLuminanceInFrame(ofPixels& pixels, ofColor& color, float* luminance);
	static void calculateAverageColorAndLuminance(vector<ofPixels>& frames, ofColor& color, float* luminance);
	static vector<ofPixels> extractFrames(ofVideoPlayer& videoPlayer, int skip);
	static int numberOfFaces(vector<ofPixels>& frames);
	static void detectEdges(ofPixels& pixels, ofXml& XML);
	static void calculateStats(const string& filterName, Mat& filteredMat, ofXml& XML, bool thresholdOn);
	static void detectTextureCharacteristics(ofPixels& pixels, ofXml& XML);
};

