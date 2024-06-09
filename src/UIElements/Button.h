#pragma once

#include "ofMain.h"

class Button
{
public:

	void setup(const std::string& iconPath, int radius, int x, int y);
	void update();
	void draw();
	void timedDraw();
	void mouseReleased(int x, int y, int button);
	void loadIcon(const std::string& iconPath);

	bool inside(int x, int y);
	int radius, x, y;
	ofImage icon;
	ofEvent<void> clickedInside;
	uint64_t startTime; // To track the start time
	float fadeAlpha;    // To track the alpha value for fading
};
