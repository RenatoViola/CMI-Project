#pragma once

#include "ofMain.h"
#include "DataTypes/Media.h"

class MediaCircle
{
public:
	~MediaCircle() {
		exit();
	}

	void setup(vector<string>& paths, int radius, int width, int height);
	void draw();
	void update();
	void exit();
	void mouseReleased(int x, int y, int button);
	string getCurrentFilePath();
	int getCurrentIndex();
	void updateBoundingBoxes();
	void setVersions(const string& filePath);

	int radius, width, height;

	double angleInRadians;

	int selectedMedia;
	ofEvent<void> clickedOnItem;

	vector<ofRectangle> boundingBoxes;
	vector<unique_ptr<Media>> items;
	vector<tuple<int, string, string>> versions;
};