#include "ScreenSaver.h"

void ScreenSaver::setup()
{
	backgroundImage.load(PATH);
}

void ScreenSaver::draw() 
{
	ofSetColor(ofColor::black);
	backgroundImage.draw(0,0,ofGetWidth(), ofGetHeight());


	float iWidth = backgroundImage.getWidth(), iHeight = backgroundImage.getHeight();

	float sWidth = ofGetWidth(), sHeight = ofGetHeight();

	ofSetColor(ofColor::white);
	ofDrawRectangle(0, 0, sWidth, sHeight);

	float scale = 1.0f;

	if (iWidth > sWidth || iHeight > sHeight)
		scale = std::min(sWidth / iWidth, sHeight / iHeight);

	float displayWidth = iWidth * scale, displayHeight = iHeight * scale;

	float xPos = (sWidth - displayWidth) / 2.0f, yPos = (sHeight - displayHeight) / 2.0f;

	ofSetColor(ofColor::white);
	backgroundImage.draw(xPos, yPos, displayWidth, displayHeight);

}

void ScreenSaver::update()
{
	// check for movement and close
}
