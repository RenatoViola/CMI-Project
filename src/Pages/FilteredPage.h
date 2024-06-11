#pragma once

#include "ofMain.h"
#include <Button.h>
#include <MediaCircle.h>
#include <Metadata.h>
#include <ImageMedia.h>
#include <VideoMedia.h>
#include <screen_names.h>

#define MAX_BLOBS 50

class FilteredPage
{
public:
	void setup(ofPixels& frame);
	void update();
	void draw();
	string getCurrentFilePath();
	void exit();
	void mouseReleased(int x, int y, int button);

	// Redirection
	ofEvent<int> redirectEvent;

	
private:
	void setupGridAreas();
	void checkBlobs();
	void trackBlobs();
	void filterBlobs(float minBlobSize);
	void gotoHomePage();
	void gotoFilePage();
	void moveToFilePage(const string& path);

	const int CAMERA_WIDTH = 1280, CAMERA_HEIGHT = 720, DISPLAY_CAMERA_WIDTH = 320, DISPLAY_CAMERA_HEIGHT = 240;
	const int TIME_WINDOW = 1000, BLOB_THRESHOLD = 50;
	float displayWidth, displayHeight, xPos, yPos, scaleX, scaleY;

	// Files to display
	string selectedFilePath;
	vector<string> matching_paths;
	MediaCircle mediaCir;

	// Camera
	ofVideoGrabber vidGrabber;
	ofxCvHaarFinder finder;
	ofxCvColorImage colorImg;
	ofxCvGrayscaleImage grayImg;

	// Motion recognition
	ofRectangle gridAreas[3][3];
	int blobCount[3][3];
	unsigned long long blobTimestamps[3][3][MAX_BLOBS];
	
	// Buttons
	Button homeBtn;
};

