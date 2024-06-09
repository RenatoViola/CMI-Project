#pragma once
#include "ofMain.h"
#include "screen_names.h"
#include <DataTypes/Media.h>
#include <DataTypes/ImageMedia.h>
#include <DataTypes/VideoMedia.h>

class Carrousel {
public:
	~Carrousel() {
		exit();
	}

	void setup(string directory, int verticalPos, bool isImageCarrousel);
	void exit();
	void draw();
	void update();
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void next();
	void previous();
	string getCurrentFilePath();

	const int width = 320, height = 240, gridSpacing = 30, numFilesToShow = 5;
	int totalWidth = numFilesToShow * width + (numFilesToShow - 1) * gridSpacing;
	int current = 0, currentWidth = 40, currentHeight = 30;
	int horizontalMiddle = (ofGetHeight() - height) / 2;
	int space = width + gridSpacing;
	int startX = (ofGetWidth() - totalWidth) / 2;
	int startY, lastX;
	float step = 0;
	bool loadFromTheRight = true, isImageCarrousel;

	ofRectangle fullCarrousel, selectedFile;

	ofEvent<void> clickedOnSelected;
	vector<unique_ptr<Media>> items;

	ofTrueTypeFont myFont;
};