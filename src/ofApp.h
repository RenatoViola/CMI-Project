#pragma once

#include "ofMain.h"
#include "Carrousel.h"
#include "VideoGrabber.h"
#include "Metadata.h"
#include "ScreenSaver.h"
#include "screen_names.h"
#include <HomePage.h>
#include "ImageEditor.h"
#include "VideoEditor.h"


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

	void changeScreen(int& page);

	VideoGrabber videoGrabber;
	Metadata filesMetadata;
	ScreenSaver screenSaverPage;
	HomePage homePage;
	ImageEditor imagePage;
	VideoEditor videoPage;

	bool openedImage, openedVideo, displayCamera, detectionEnabled;

	int activePage;
};