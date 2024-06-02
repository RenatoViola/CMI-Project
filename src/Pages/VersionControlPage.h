#pragma once

#include "ofMain.h"
#include "DataTypes/Media.h"
#include "UIElements/Button.h"
#include "UIElements/MediaCircle.h"
#include <DataTypes/VideoMedia.h>
#include <screen_names.h>

//#define CAMERA_DEBUG

class VersionControlPage
{
public:
	void setup();
	void draw();
	void update();
	void exit();

	void gotoHomePage();
	void mouseReleased(int x, int y, int button);

	MediaCircle mediaCir;

	ofEvent<int> redirectEvent;
	Button homeBtn;

	#pragma region Camera Vision properties

	const int CAMERA_HEIGHT = 240;
	const int CAMERA_WIDHT = 320;
	const int THRESHOLD = 70;
	const double ALPHA = 0.02;

	ofxCvGrayscaleImage currentFrame;
	ofxCvGrayscaleImage bgImage;
	ofxCvGrayscaleImage diff;

	ofVideoGrabber videoGrabber;

	ofxCvContourFinder 	contourFinder;

	#pragma endregion

private:

	bool bLearnBackground;

	void checkForMovement();
};

