#pragma once
#include "MediaEditor.h"

class VideoEditor : public MediaEditor
{
public:

	void setup() {
		panel.setup();
	}

	void load(VideoMedia* m)
	{
		media = m;
		m->play();
	}

	void update()
	{

	}

	void draw()
	{
		media->drawInFullscreen(ofColor::black);
		panel.draw();
		// Chooses the filter to apply to the media file
	//if (panel.isAsciiActive()) {
		//media->drawInAscii();
	//} else {
		//media->drawInFullscreen(sWidth, sHeight);
	//}
	}

	void exit()
	{
	}
};