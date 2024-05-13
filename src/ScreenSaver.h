#pragma once
#include "ofMain.h"
#include <ImageMedia.h>

class ScreenSaver
{
public:
	void setup();
	void draw();
	void update();
	void exit();

	ImageMedia backgroundImage;
	const string PATH = "./screenSaver.png";
};