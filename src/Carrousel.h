#pragma once
#include "ofMain.h"
#include "screen_names.h"
#include <Media.h>
#include <ImageMedia.h>
#include <VideoMedia.h>

class Carrousel {
public:
	~Carrousel() {
		exit();
	}

	void setup(vector<unique_ptr<Media>>&& items, bool isImageCarrousel, string label);
	void exit();
	void draw();
	void update();
	void mousePressed(ofMouseEventArgs& args);
	void mouseReleased(ofMouseEventArgs& args);
	void next();
	void previous();
	Media* getCurrentMedia();

	int width = 320, height = 240, gridSpacing = 30, numFilesToShow = 5;
	int totalWidth = numFilesToShow * width + (numFilesToShow - 1) * gridSpacing;
	int current = 0, currentWidth = 40, currentHeight = 30;
	int horizontalMiddle = (ofGetHeight() - height) / 2;
	int space = width + gridSpacing;
	int startX = (ofGetWidth() - totalWidth) / 2;
	int startY, lastX;
	float step = 0;
	bool loadFromTheRight = true, isImageCarrousel;
	string label;

	ofRectangle fullCarrousel, selectedFile;

//	ofDirectory dir;
//	ofXml xml;

	ofEvent<int> onChangeScreen;
	vector<unique_ptr<Media>> items;
};