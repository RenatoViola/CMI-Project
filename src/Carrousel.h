#pragma once
#include "ofMain.h"

class Carrousel {
public:
	~Carrousel() {
		exit();
	}

	void setup(const std::string& path, bool isImageCarrousel) {
		this->isImageCarrousel = isImageCarrousel;
		ofAddListener(ofEvents().mousePressed, this, &Carrousel::mousePressed);
		ofAddListener(ofEvents().mouseReleased, this, &Carrousel::mouseReleased);
	}
	
	void exit() {
		ofRemoveListener(ofEvents().mousePressed, this, &Carrousel::mousePressed);
		ofRemoveListener(ofEvents().mouseReleased, this, &Carrousel::mouseReleased);
	}

	virtual void draw() = 0;
	virtual void displayCurrent() = 0;

	void mousePressed(ofMouseEventArgs& args) {
		lastX = args.x;
	}

	void mouseReleased(ofMouseEventArgs& args) {
		
		int x = args.x, y = args.y;

		if (selectedFile.inside(x, y))
		{
			int PAGE;
			if (isImageCarrousel)
			{
				PAGE = 5;
			}
			else {
				PAGE = 6;
			}

			ofNotifyEvent(onChangeScreen, PAGE, this);
			return;
		}

		if (!fullCarrousel.inside(x, y)) return;

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
	int startY, lastX;
	float step = 0;
	bool loadFromTheRight = true, isImageCarrousel;

	ofRectangle fullCarrousel, selectedFile;

	ofDirectory dir;
	ofXml xml;

	ofEvent<int> onChangeScreen;
};