#pragma once

#include "ofMain.h"
#include <Button.h>
#include <MediaCircle.h>
#include <Metadata.h>
#include <ImageMedia.h>
#include <VideoMedia.h>
#include <screen_names.h>

class FilteredPage
{
public:
	void setup(ofPixels& frame);
	void draw();
	void update();
	void exit();

	void gotoHomePage();
	void gotoFilePage();
	void moveToFilePage(const string& path);
	void mouseReleased(int x, int y, int button);
	string getCurrentFilePath();
	void setupGridAreas();
	void checkBlobs();
	void trackBlobs();
	void filterBlobs(float minBlobSize);
	void triggerEvent(int row, int col);


	string selectedFilePath;

	MediaCircle mediaCir;

	ofEvent<int> redirectEvent;
	Button homeBtn;

	
	const int CAMERA_WIDTH = 1280, CAMERA_HEIGHT = 720, DISPLAY_CAMERA_WIDTH = 320, DISPLAY_CAMERA_HEIGHT = 240;

	float displayWidth, displayHeight, xPos, yPos, scaleX, scaleY;

	ofVideoGrabber vidGrabber;
	ofxCvHaarFinder finder;
	ofxCvColorImage colorImg;
	ofxCvGrayscaleImage grayImg;

	const int TIME_WINDOW = 1000; // Time window in milliseconds (1 second)
	const int BLOB_THRESHOLD = 1; // Threshold for number of blobs to trigger the event
	const int MAX_BLOBS = 50;

	ofRectangle gridAreas[3][3];
	int blobCount[3][3];
	unsigned long long blobTimestamps[3][3][50];

	vector<string> matching_paths;
};

