#pragma once

#include "ofMain.h"
#include "DataTypes/Media.h"
#include "UIElements/Button.h"
#include "UIElements/MediaCircle.h"
#include <screen_names.h>
#include <Metadata.h>
#include <ImageMedia.h>
#include <VideoMedia.h>

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

	const int FILE_HEIGHT = 240;
	const int FILE_WIDTH = 320;

private:
	string selectedFilePath;
	ofxCvColorImage img;
};

