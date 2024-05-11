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

	int width = 320, height = 240, gridSpacing = 30;
	int current = 0, currentWidth = 40, currentHeight = 30;
	int horizontalMiddle = (ofGetHeight() - height) / 2; // Horizontal middle of the screen
	float step = 0;
	int lastX;
	bool loadFromTheRight = true;

	

	ofDirectory dir;
	ofXml xml;
};