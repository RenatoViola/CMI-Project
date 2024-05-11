#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(255, 255, 255);
	ofSetVerticalSync(true);


	// setup Screen Saver Page
	screenSaverPage.setup();

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
	detectionEnabled = false;

	activePage = 0;

	ofSetVerticalSync(true);
}

//--------------------------------------------------------------
void ofApp::update() {

	switch (activePage)
	{
	case SCREEN_SAVER_PAGE:
		screenSaverPage.update();
		break;
	case MAIN_PAGE:
		videoCarrousel.update();
		break;
	case FILTERED_PAGE:
		break;
	case CAMERA_PAGE:
		videoGrabber.update(detectionEnabled);
		break;
	case CONTROL_VERSION_PAGE:
		break;
	default:
		break;
	}


	if (activePage != CAMERA_PAGE)
	{
		detectionEnabled = false;
	}
}

void ofApp::draw() {

	switch (activePage)
	{
	case SCREEN_SAVER_PAGE:
		screenSaverPage.draw();
		break;
	case MAIN_PAGE:
		imageCarrousel.draw(5, 320, 240, 30);
		videoCarrousel.draw(5, 320, 240, 30);
		break;
	case FILTERED_PAGE:
		break;
	case CAMERA_PAGE:
		videoGrabber.drawCamera(detectionEnabled);
		break;
	case CONTROL_VERSION_PAGE:
		break;
	case IMAGE_PAGE:
		imageCarrousel.displayCurrent();
		break;
	case VIDEO_PAGE:
		videoCarrousel.displayCurrent();
		break;
	default:
		break;
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
		if (activePage == MAIN_PAGE)
			activePage = VIDEO_PAGE;
		else if (activePage == VIDEO_PAGE)
			activePage = MAIN_PAGE;

		//if(!openedImage) openedVideo = !openedVideo;
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
		if (activePage == MAIN_PAGE)
			activePage = IMAGE_PAGE;
		else if (activePage == IMAGE_PAGE)
			activePage = MAIN_PAGE;
		//if(!openedVideo) openedImage = !openedImage;
		break;
	case 'c':
		activePage = CAMERA_PAGE;
		break;
	case OF_KEY_RETURN:
		if (activePage == SCREEN_SAVER_PAGE)
			activePage = MAIN_PAGE;
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
