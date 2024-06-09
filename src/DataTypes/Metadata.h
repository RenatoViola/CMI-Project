#pragma once

#include "ofMain.h"
#include "ofxCvHaarFinder.h"
#include "../metadata_struct.h"
using namespace cv;

class Metadata
{
public:

	static void load(string filePath);
	static void save(string filePath, ofXml& XML);
	static set<string> getTags(ofXml& XML);
	static vector<string> filesWithObject(ofPixels& pixels, const vector<string>& image_paths, const vector<string>& video_paths);
	static vector<string> findRelatedFiles(string path, vector<string>& image_paths, vector<string>& video_paths);

	static void processFileMetadata(string fileName, vector<ofPixels>& frames, ofXml& XML);
	static void calculateAverageColorAndLuminanceInFrame(ofPixels& pixels, ofColor& color, float* luminance);
	static void calculateAverageColorAndLuminance(vector<ofPixels>& frames, ofXml& XML);
	static int numberOfFaces(vector<ofPixels>& frames);
	static void detectEdges(ofPixels& pixels, ofXml& XML);
	static void calculateStats(const string& filterName, Mat& filteredMat, ofXml& XML, bool thresholdOn);
	static void detectTextureCharacteristics(ofPixels& pixels, ofXml& XML);
	
	static int countOccurrencesInFrame(ofPixels& pixels, Mat& desc1, vector<KeyPoint>& kpts1);

	static FileMetadata parseMetadata(const string& filePath);
	static double calculateSimilarity(const FileMetadata& file1, const FileMetadata& file2);

	static vector<pair<int, string>> getVersionedRelatedFiles(const string& filePath, const vector<string>& relatedFilenames);
	static vector<tuple<int, string, string>> getVersionedDates(const string& filePath);
	
	// SAVE VERSION
	// GET VERSIONS AND RELATED FILES
};

