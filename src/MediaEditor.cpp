#include "MediaEditor.h"

void MediaEditor::setup()
{
	//media = make_unique<Media>();
	panel.setup();
}

void MediaEditor::load(Media* m)
{
	media = m;
}

void MediaEditor::update()
{

}

void MediaEditor::draw()
{
	float sWidth = ofGetWidth(), sHeight = ofGetHeight();

	ofSetColor(ofColor::black);
	ofDrawRectangle(0, 0, sWidth, sHeight);

	float scale = 1.0f;
	media->drawInFullscreen(sWidth, sHeight);
	
	// Chooses the filter to apply to the media file
	//if (panel.isAsciiActive()) {
		//media->drawInAscii();
	//} else {
		//media->drawInFullscreen(sWidth, sHeight);
	//}
}