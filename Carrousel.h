#pragma once
#include "ofMain.h"

class Carrousel {
public:
	virtual ~Carrousel() {}

	virtual void setup(const std::string& path) = 0;
	virtual void update() = 0;
	virtual void draw(int numFilesToShow, int width, int height, int gridSpacing) = 0;

	virtual void next() = 0;
	virtual void previous() = 0;

	int displayWidth;
	int displayHeight;
	int current;

	ofDirectory dir;
};