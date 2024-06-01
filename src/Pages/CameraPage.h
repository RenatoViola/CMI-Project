#pragma once
#include "ofMain.h"
#include "ofxCvHaarFinder.h"
#include <UIElements/Button.h>

class CameraPage {

	public:

		void setup(int width, int height);
		void update(bool detectionEnabled);
		void drawCamera(bool detectionEnabled);
		void exit();
		void mouseReleased(int x, int y, int button);
		void gotoPreviousPage();

		ofEvent<int> redirectEvent;

		ofVideoGrabber vidGrabber;

		ofxCvHaarFinder finder;
		ofxCvColorImage colorImg;
		ofxCvGrayscaleImage grayImg;

		vector <ofRectangle> faceRects;

		int camWidth;
		int camHeight;

		Button backBtn;
};