#pragma once
#include "ofMain.h"
#include <ofxOpenCv.h>

class ScreenSaver
{
public:
	void setup();
	void draw();
	void update();
	void exit();

	const string PATH = "./screenSaver.png";
	ofImage img;
};