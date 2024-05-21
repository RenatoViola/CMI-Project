#pragma once

#include "ofMain.h"

class Button
{
public:

	void setup(const std::string& iconPath, int radius, int x, int y);
	void draw();
	void mouseReleased(int x, int y, int button);

	bool inside(int x, int y);
	int radius, x, y;
	ofImage icon;
	ofEvent<void> clickedInside;
};
