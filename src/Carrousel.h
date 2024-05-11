#pragma once
#include "ofMain.h"

class Carrousel {
public:
	virtual ~Carrousel() {
		exit();
	}

	virtual void setup(const std::string& path) {
		ofAddListener(ofEvents().mousePressed, this, &Carrousel::mousePressed);
		ofAddListener(ofEvents().mouseReleased, this, &Carrousel::mouseReleased);
	}
	
	virtual void exit() {
		ofRemoveListener(ofEvents().mousePressed, this, &Carrousel::mousePressed);
		ofRemoveListener(ofEvents().mouseReleased, this, &Carrousel::mouseReleased);
	}

	virtual void draw() = 0;
	virtual void displayCurrent() = 0;

	virtual void mousePressed(ofMouseEventArgs& args) {
		lastX = args.x;
	}

	virtual void mouseReleased(ofMouseEventArgs& args) {

		if (!fullCarrousel.inside(args.x, args.y)) return;

		int deltaX = args.x - lastX;

		int threshold = 50;

		if (deltaX < -threshold) { // Dragged from right to left
			next();
		}
		else if (deltaX > threshold) { // Dragged from left to right
			previous();
		}
	}
	
	virtual void next() = 0;
	virtual void previous() = 0;

	int vWidth = 320, vHeight = 240, gridSpacing = 30, numFilesToShow = 5;
	int totalWidth = numFilesToShow * vWidth + (numFilesToShow - 1) * gridSpacing;
	int current = 0, currentWidth = 40, currentHeight = 30;
	int horizontalMiddle = (ofGetHeight() - vHeight) / 2;
	int space = vWidth + gridSpacing;
	int startX = (ofGetWidth() - totalWidth) / 2;
	int startY;
	float step = 0;
	int lastX;
	bool loadFromTheRight = true;

	ofRectangle fullCarrousel, selectedFile;

	ofDirectory dir;
	ofXml xml;
};