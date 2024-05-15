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

	const string PATH = "./screenSaver.png";
	const int CAMERA_HEIGHT = 240;
	const int CAMERA_WIDHT = 320;

	const int THRESHOLD = 70;
	ofImage img;

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