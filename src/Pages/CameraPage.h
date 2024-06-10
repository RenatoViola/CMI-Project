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
	ofPixels getCapturedFrame() const;

	ofEvent<int> redirectEvent;

private:
	void gotoHomePage();
	void initiateSearchResultPage();
	void gotoSearchResultPage();
	void toggleFaceDetection();

	int camWidth;
	int camHeight;
	bool facialDetectionEnabled, showLoadingMessage = false, isRedirecting = false;

	ofVideoGrabber vidGrabber;
	ofPixels capturedFrame;

	ofxCvHaarFinder finder;
	ofxCvColorImage colorImg;
	ofxCvGrayscaleImage grayImg;

	vector <ofRectangle> faceRects;

	Button homeBtn, faceBtn, searchBtn;
	uint64_t startTime;

	ofTrueTypeFont myFont;
};