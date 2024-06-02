#pragma once

#include "ofMain.h"
#include <ofxOpenCv.h>

//#define CAMERA_DEBUG		// uncommented to draw the diff image

class ScreenSaver
{
public:
	void setup();
	void draw();
	void update();
	void exit();

	const string PATH = "./screenSaver.jpg";
	const int CAMERA_HEIGHT = 240;
	const int CAMERA_WIDTH = 320;

	const int THRESHOLD = 70;
	const double ALPHA = 0.05;
	ofImage img;
	ofxCvColorImage colorImg;

	ofxCvGrayscaleImage currentFrame;
	ofxCvGrayscaleImage bgImage;
	ofxCvGrayscaleImage diff;
	
	ofVideoGrabber videoGrabber;

	ofEvent<int> redirectEvent;

private:

	bool bLearnBackground , changeScreen;
	int counterToLearn;

	void checkForMovement();
};