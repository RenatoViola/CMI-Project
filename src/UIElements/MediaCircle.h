#pragma once

#include "ofMain.h"
#include "DataTypes/Media.h"

class MediaCircle
{
public:
	~MediaCircle() {
		exit();
	}

	void setup(vector<string> paths);
	void draw();
	void update();
	void exit();
	void mouseReleased(int x, int y, int button);
	string getCurrentFilePath();
	int getCurrentIndex();
	void updateBoundingBoxes();

	const int MEDIA_WIDTH = 240;
	const int MEDIA_HEIGHT = 180;

	const int CIRCLE_RADIUS = 400;

	double angleInRadians;

	int selectedMedia;
	ofEvent<void> clickedOnItem;

	vector<ofRectangle> boundingBoxes;
	vector<unique_ptr<Media>> items;
};