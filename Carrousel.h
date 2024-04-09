#pragma once
#include "ofMain.h"

class Carrousel {
public:
	virtual ~Carrousel() {}

	virtual void setup(const std::string& path) = 0;
	virtual void drawAsList(int numFilesToShow, int width, int height, int gridSpacing) = 0;
	virtual void drawAsRevolver(int radius, int width, int height) = 0;
	virtual void displayCurrent() = 0;

	virtual void next() = 0;
	virtual void previous() = 0;

	int current;

	ofDirectory dir;
};