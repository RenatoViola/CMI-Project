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
	void mouseReleased(int x, int y, int button);
	string getCurrentFilePath();
	void setupGridAreas();
	void checkBlobs();
	void filterBlobs(float minBlobSize);


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

	ofRectangle gridAreas[3][3];
};

