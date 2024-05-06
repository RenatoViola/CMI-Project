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
	
	// Ignore this, used for debugging
	//filesMetadata.getFileTags();
	//filesMetadata.createFile("other.xml", { "9s","2b" });

	openedImage = false;
	openedVideo = false;
	displayCamera = false;
	detectionEnabled = false;

	ofSetVerticalSync(true);
}

//--------------------------------------------------------------
void ofApp::update() {
	videoCarrousel.update();

	if (displayCamera)
	{
		videoGrabber.update(detectionEnabled);
	}
	else
	{
		detectionEnabled = false;
	}
}

void ofApp::draw() {

	if (displayCamera) 
	{
		videoGrabber.drawCamera(detectionEnabled);
	}
	else if (openedImage)
	{
		imageCarrousel.displayCurrent();
	}
	else if (openedVideo) 
	{
		videoCarrousel.displayCurrent();
	}
	else {
		imageCarrousel.draw(5, 320, 240, 30);
		videoCarrousel.draw(5, 320, 240, 30);
	}
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key) {
	case 'f':
		detectionEnabled = !detectionEnabled;
		break;
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
	case 'c':
		displayCamera = !displayCamera;
		break;
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
