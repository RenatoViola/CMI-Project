#pragma once

#include "ofMain.h"
#include "ofxCvHaarFinder.h"
#include "../metadata_struct.h"
using namespace cv;

class Metadata
{
public:

	static void load(string filename, ofXml& XML, bool isImage);
	static void save(string filename, ofXml& XML, bool isImage);
	static vector<string> getTags(ofXml& XML);
	static vector<string> filesWithObject(ofPixels& pixels, vector<string>& image_paths, vector<string> video_paths);

	static void processFileMetadata(string fileName, vector<ofPixels>& frames, ofXml& XML);
	static void calculateAverageColorAndLuminanceInFrame(ofPixels& pixels, ofColor& color, float* luminance);
	static void calculateAverageColorAndLuminance(vector<ofPixels>& frames, ofXml& XML);
	static int numberOfFaces(vector<ofPixels>& frames);
	static void detectEdges(ofPixels& pixels, ofXml& XML);
	static void calculateStats(const string& filterName, Mat& filteredMat, ofXml& XML, bool thresholdOn);
	static void detectTextureCharacteristics(ofPixels& pixels, ofXml& XML);
	
	static int countOccurrencesInFrame(ofPixels& pixels, Mat& desc1, vector<KeyPoint>& kpts1);
	static bool comparePairs(const std::pair<std::string, int>& a, const std::pair<std::string, int>& b);

	static FileMetadata parseMetadata(const string& filename);
	static double calculateSimilarity(const FileMetadata& file1, const FileMetadata& file2);
	
};

