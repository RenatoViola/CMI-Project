#pragma once
#include "ofMain.h"

class VideoGrabber {

	public:

		void setup(int width, int height);
		void update();
		void drawCamera();


		ofVideoGrabber vidGrabber;
		ofPixels videoInverted;
		ofTexture videoTexture;
		int camWidth;
		int camHeight;
};