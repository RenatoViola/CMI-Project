#pragma once

#include "ofMain.h"
#include <ofxOpenCv.h>
#include <DataTypes/Media.h>
#include <screen_names.h>

class ScreenSaver
{
public:
	void setup();
	void draw();
	void update();
	void exit();

	ofEvent<int> redirectEvent;

private:
	void checkForMovement();

	bool bLearnBackground , changeScreen;
	int counterToLearn;

	const string PATH = "./screenSaver.jpg";
	const int CAMERA_HEIGHT = 240, CAMERA_WIDTH = 320, THRESHOLD = 70;
	const double ALPHA = 0.01;

	ofImage img;
	ofxCvColorImage colorImg;

	ofxCvGrayscaleImage currentFrame;
	ofxCvGrayscaleImage bgImage;
	ofxCvGrayscaleImage diff;

	ofVideoGrabber videoGrabber;
};