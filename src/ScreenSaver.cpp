#include "ScreenSaver.h"

void ScreenSaver::setup()
{
	backgroundImage.load(PATH);
}

void ScreenSaver::draw() 
{
	backgroundImage.drawInFullscreen(ofColor::white);
}

void ScreenSaver::update()
{
	// check for movement and close
}

void ScreenSaver::exit() {
	backgroundImage.exit();
}
