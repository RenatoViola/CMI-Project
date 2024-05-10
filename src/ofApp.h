#pragma once

#include "ofMain.h"
#include "ImageCarrousel.h"
#include "VideoCarrousel.h"
#include "VideoGrabber.h"
#include "Metadata.h"
#include "ScreenSaver.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	ImageCarrousel imageCarrousel;
	VideoCarrousel videoCarrousel;
	VideoGrabber videoGrabber;
	Metadata filesMetadata;
	ScreenSaver screenSaverPage;

	bool openedImage, openedVideo, displayCamera, detectionEnabled;

	int activePage;

	// interface pages enum
	enum {
		SCREEN_SAVER_PAGE = 0,
		MAIN_PAGE = 1,
		FILTERED_PAGE = 2,
		CAMERA_PAGE = 3,
		CONTROL_VERSION_PAGE = 4,
		IMAGE_PAGE = 5,
		VIDEO_PAGE = 6
		// Must add other pages here
	};
};