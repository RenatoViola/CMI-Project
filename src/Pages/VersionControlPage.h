#pragma once

#include "ofMain.h"
#include "DataTypes/Media.h"

class VersionControlPage
{
public:
	void setup();
	void draw();
	void update();
	void exit();

	const int CAMERA_HEIGHT = 240;
	const int CAMERA_WIDHT = 320;

	vector<Media> media;


};

