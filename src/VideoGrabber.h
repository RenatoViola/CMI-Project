#pragma once
#include "ofMain.h"
#include "ofxCvHaarFinder.h"

class VideoGrabber {

	public:

		void setup(int width, int height);
		void update(bool detectionEnabled);
		void drawCamera(bool detectionEnabled);


		ofVideoGrabber vidGrabber;

		ofxCvColorImage	colorImg;
		ofxCvHaarFinder finder;
		ofImage img;

		vector <ofRectangle> faceRects;

		int camWidth;
		int camHeight;
};