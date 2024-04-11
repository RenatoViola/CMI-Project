#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(255, 255, 255);
	ofSetVerticalSync(true);

	// Uncomment this to show movies with alpha channels
	// videoPlayer.setPixelFormat(OF_PIXELS_RGBA);
	
	imageCarrousel.setup("images/");
	videoCarrousel.setup("videos/");
	videoGrabber.setup(1280, 720);

	openedImage = false;
	openedVideo = false;
	displayAsList = true;
	displayCamera = false;

	ofSetVerticalSync(true);
}

//--------------------------------------------------------------
void ofApp::update() {
	videoCarrousel.update();
	videoGrabber.update();
}

void ofApp::draw() {
	if (!openedImage && ! openedVideo && !displayCamera)
	{
		if (displayAsList) {
			imageCarrousel.drawAsList(5, 320, 240, 30);
			videoCarrousel.drawAsList(5, 320, 240, 30);
		}
		else {
			imageCarrousel.drawAsRevolver(250, 120, 90);
			videoCarrousel.drawAsRevolver(450, 120, 90);
		}
	}
	else if (displayCamera) {
		videoGrabber.drawCamera();
	}
	else if (openedImage)
	{
		imageCarrousel.displayCurrent();
	}
	else {
		videoCarrousel.displayCurrent();
	}
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key) {
	case 'p':
		videoCarrousel.pause();
		break;
	case 'b':
		videoCarrousel.previous();
		break;
	case 'n':
		videoCarrousel.next();
		break;
	case 'm':
		if(!openedImage) openedVideo = !openedVideo;
		break;
	case OF_KEY_LEFT:
		videoCarrousel.previousFrame();
		break;
	case OF_KEY_RIGHT:
		videoCarrousel.nextFrame();
		break;
	case '0':
		videoCarrousel.firstFrame();
		break;
	case 'h':
		imageCarrousel.previous();
		break;
	case 'j':
		imageCarrousel.next();
		break;
	case 'k':
		if(!openedVideo) openedImage = !openedImage;
		break;
	case 'd':
		displayAsList = !displayAsList;
		break;
	case 'c':
		displayCamera = !displayCamera;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
