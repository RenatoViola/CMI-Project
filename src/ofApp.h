#pragma once

#include "ofMain.h"
//#include "ImageCarrousel.h"
//#include "VideoCarrousel.h"
#include "Carrousel.h"
#include "VideoGrabber.h"
#include "Metadata.h"
#include "ScreenSaver.h"
#include "screen_names.h"


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

//	ImageCarrousel imageCarrousel;
//	VideoCarrousel videoCarrousel;
	VideoGrabber videoGrabber;
	Metadata filesMetadata;
	ScreenSaver screenSaverPage;

	bool openedImage, openedVideo, displayCamera, detectionEnabled;

	int activePage;

	Carrousel imageMediaCarrousel, videoMediaCarrousel;	
};