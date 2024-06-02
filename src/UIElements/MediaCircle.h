#pragma once

#include "ofMain.h"
#include "DataTypes/Media.h"

class MediaCircle
{
public:

	void setup(vector<unique_ptr<Media>>&& items);
	void draw();
	void update();
	void exit();

	Media* getSelectedMedia();

	const int MEDIA_WIDTH = 320;
	const int MEDIA_HEIGHT = 240;

	const int CIRCLE_RADIUS = 450;

	double angleInRadians;

	int selectedMedia;
	vector<unique_ptr<Media>> items;
};

