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

	void mouseReleased(int x, int y, int button);
	string getCurrentFilePath();
	int getCurrentVersion();

	ofEvent<int> redirectEvent;

private:
	void gotoHomePage();
	void gotoFilePage();

	const int FILE_HEIGHT = 240, FILE_WIDTH = 320;
	string selectedFilePath;
	int selectedVersion;
	vector<pair<int, string>> files;
	ofxCvColorImage img;
	MediaCircle mediaCir;
	Button homeBtn;
};