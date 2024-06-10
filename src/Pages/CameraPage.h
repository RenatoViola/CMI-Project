#pragma once
#include "ofMain.h"
#include "ofxCvHaarFinder.h"
#include <UIElements/Button.h>

class CameraPage {

	public:

		void setup(int width, int height);
		void update();
		void drawCamera();
		void exit();
		void mouseReleased(int x, int y, int button);
		void gotoHomePage();
		void initiateSearchResultPage();
		void gotoSearchResultPage();
		void toggleFaceDetection();
		ofPixels getCapturedFrame() const;

		ofEvent<int> redirectEvent;

		ofVideoGrabber vidGrabber;
		ofPixels capturedFrame;

		ofxCvHaarFinder finder;
		ofxCvColorImage colorImg;
		ofxCvGrayscaleImage grayImg;

		vector <ofRectangle> faceRects;

		int camWidth;
		int camHeight;
		bool facialDetectionEnabled;

		Button homeBtn, faceBtn, searchBtn;
		bool showLoadingMessage = false;
		uint64_t startTime;
		bool isRedirecting = false;

		ofTrueTypeFont myFont;
};