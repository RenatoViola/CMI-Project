#pragma once
#include "ofMain.h"
#include "ofxCvHaarFinder.h"

class VideoGrabber {

	public:

		void setup(int width, int height);
		void update(bool detectionEnabled);
		void drawCamera(bool detectionEnabled);


		ofVideoGrabber vidGrabber;

		ofxCvHaarFinder finder;
		ofxCvColorImage colorImg;
		ofxCvGrayscaleImage grayImg;

		vector <ofRectangle> faceRects;

		int camWidth;
		int camHeight;
};