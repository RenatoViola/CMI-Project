#pragma once
#include "ofMain.h"

class Carrousel {
public:
	virtual ~Carrousel() {}

	virtual void setup(const std::string& path) = 0;
	virtual void exit() = 0;
	virtual void draw() = 0;
	virtual void displayCurrent() = 0;

	virtual void mousePressed(ofMouseEventArgs& args) = 0;
	virtual void mouseReleased(ofMouseEventArgs& args) = 0;
	
	virtual void next() = 0;
	virtual void previous() = 0;

	int vWidth = 320, vHeight = 240, gridSpacing = 30, numFilesToShow = 5;
	int totalWidth = numFilesToShow * vWidth + (numFilesToShow - 1) * gridSpacing;
	int current = 0, currentWidth = 40, currentHeight = 30;
	int horizontalMiddle = (ofGetHeight() - vHeight) / 2;
	int space = vWidth + gridSpacing;
	int startX = (ofGetWidth() - totalWidth) / 2;
	int xPos = startX + 2 * space;
	int startY;
	float step = 0;
	int lastX;
	bool loadFromTheRight = true;

	ofRectangle fullCarrousel, selectedFile;

	ofDirectory dir;
	ofXml xml;
};