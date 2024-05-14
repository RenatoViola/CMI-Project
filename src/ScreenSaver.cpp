#include "ScreenSaver.h"
#include <Media.h>

void ScreenSaver::setup()
{
	img.load(PATH);
}

void ScreenSaver::draw() 
{
	Media::drawInFullscreen(img.getPixels(), ofColor::white);
}

void ScreenSaver::update()
{
	// check for movement and close
}

void ScreenSaver::exit() {
//	backgroundImage.clear();
}
