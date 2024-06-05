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
	string selectedFilePath;

	MediaCircle mediaCir;

	ofEvent<int> redirectEvent;
	Button homeBtn;

#pragma region Camera Vision properties

	const int CAMERA_HEIGHT = 240;
	const int CAMERA_WIDTH = 320;
	const int THRESHOLD = 70;
	const double ALPHA = 0.001;

	ofxCvGrayscaleImage currentFrame;
	ofxCvGrayscaleImage bgImage;
	ofxCvGrayscaleImage diff;

	ofVideoGrabber videoGrabber;

	ofxCvContourFinder 	contourFinder;

#pragma endregion

	bool bLearnBackground;
	void checkForMovement();

	ofxCvColorImage img;
};

