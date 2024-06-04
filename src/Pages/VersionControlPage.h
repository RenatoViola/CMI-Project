#pragma once

#include "ofMain.h"
#include "DataTypes/Media.h"
#include "UIElements/Button.h"
#include "UIElements/MediaCircle.h"
#include <DataTypes/VideoMedia.h>
#include <screen_names.h>

#define CAMERA_DEBUG

class VersionControlPage
{
public:
	void setup(string filepath);
	void draw();
	void update();
	void exit();

	void gotoHomePage();
	void gotoFilePage();
	void mouseReleased(int x, int y, int button);
	string getCurrentFilePath();

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

private:

	bool bLearnBackground;
	string selectedFilePath;
	void checkForMovement();
};

