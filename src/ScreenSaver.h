#pragma once
#include "ofMain.h"

class ScreenSaver
{
public:
	virtual void setup();
	virtual void draw();

	void update();

	ofImage backgroundImage;
	const string PATH = "./screenSaver.png";

};