#pragma once

#include "ofMain.h"
#include "DataTypes/Media.h"
#include <ImageMedia.h>
#include <VideoMedia.h>
#include <Metadata.h>


class MediaCircle
{
public:
	~MediaCircle() {
		exit();
	}

	void setup(vector<string>& paths, int radius, int width, int height);
	void draw();
	void update();
	string getCurrentFilePath();
	int getCurrentIndex();
	void setVersions(const string& filePath);
	void exit();
	void mouseReleased(int x, int y, int button);

	ofEvent<void> clickedOnItem;
	
private:
	void updateBoundingBoxes();
	
	int radius, width, height, selectedMedia;
	double angleInRadians;
	vector<ofRectangle> boundingBoxes;
	vector<unique_ptr<Media>> items;
	vector<tuple<int, string, string>> versions;
	ofTrueTypeFont myFont;
};