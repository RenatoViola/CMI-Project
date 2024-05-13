#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(255, 255, 255);
	ofSetVerticalSync(true);

	// setup Screen Saver Page
	screenSaverPage.setup();
	homePage.setup();

	// Uncomment this to show movies with alpha channels
	// videoPlayer.setPixelFormat(OF_PIXELS_RGBA);

	videoGrabber.setup(1280, 720);
	detectionEnabled = false;
	
	activePage = SCREEN_SAVER_PAGE;

	ofSetVerticalSync(true);

	ofAddListener(homePage.clickedOnMedia, this, &ofApp::changeScreen);
}

//--------------------------------------------------------------
void ofApp::update() {

	switch (activePage)
	{
		case SCREEN_SAVER_PAGE:
			screenSaverPage.update();
			break;
		case MAIN_PAGE:
			homePage.update();
			break;
		case FILTERED_PAGE:
			break;
		case CAMERA_PAGE:
			videoGrabber.update(detectionEnabled);
			break;
		case CONTROL_VERSION_PAGE:
			break;
		case IMAGE_PAGE:
			imagePage.update();
			break;
		case VIDEO_PAGE:
			videoPage.update();
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
		homePage.draw();
		break;
	case FILTERED_PAGE:
		break;
	case CAMERA_PAGE:
		videoGrabber.drawCamera(detectionEnabled);
		break;
	case CONTROL_VERSION_PAGE:
		break;
	case IMAGE_PAGE:
	//	imageCarrousel.displayCurrent();
		mEditor.draw();
		break;
	case VIDEO_PAGE:
	//	Media* selectedMedia = videoMediaCarrousel.getCurrentMedia();
		mEditor.draw();
		
		//video->play();
	//	videoCarrousel.displayCurrent();
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
	case 'm':
		if (activePage == MAIN_PAGE) {
			activePage = VIDEO_PAGE;
			mEditor.media = videoMediaCarrousel.getCurrentMedia();
		}
		else if (activePage == VIDEO_PAGE)
			activePage = MAIN_PAGE;

		//if(!openedImage) openedVideo = !openedVideo;
		break;
	case 'k':
		if (activePage == MAIN_PAGE) {
			activePage = IMAGE_PAGE;
			mEditor.media = imageMediaCarrousel.getCurrentMedia();
		}
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

void ofApp::changeScreen(int& page) {
	switch (activePage)
	{
		// Close previous page
		case SCREEN_SAVER_PAGE:
			break;
		case MAIN_PAGE:
			homePage.exit();
			break;
		case FILTERED_PAGE:
			break;
		case CAMERA_PAGE:
			break;
		case CONTROL_VERSION_PAGE:
			break;
		case IMAGE_PAGE:
			imagePage.exit();
			break;
		case VIDEO_PAGE:
			videoPage.exit();
			break;
		default:
			break;
	}

	activePage = page;

	// Setup the new one
	switch (activePage)
	{
	case SCREEN_SAVER_PAGE:
		screenSaverPage.setup();
		break;
	case MAIN_PAGE:
		homePage.setup();
		break;
	case FILTERED_PAGE:
		break;
	case CAMERA_PAGE:
		videoGrabber.setup(1280, 720);
		break;
	case CONTROL_VERSION_PAGE:
		break;
	case IMAGE_PAGE:
		imagePage.setup(dynamic_cast<ImageMedia*>(homePage.getSelectedMedia()));
		break;
	case VIDEO_PAGE:
		videoPage.setup(dynamic_cast<VideoMedia*>(homePage.getSelectedMedia()));
		break;
	default:
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

	switch (activePage)
	{
		case SCREEN_SAVER_PAGE:
			break;
		case MAIN_PAGE:
			homePage.mousePressed(x, y, button);
			break;
		case FILTERED_PAGE:
			break;
		case CAMERA_PAGE:
			break;
		case CONTROL_VERSION_PAGE:
			break;
		case IMAGE_PAGE:
			break;
		case VIDEO_PAGE:
			break;
		default:
			break;
	}
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

	switch (activePage)
	{
	case SCREEN_SAVER_PAGE:
		break;
	case MAIN_PAGE:
		homePage.mouseReleased(x, y, button);
		break;
	case FILTERED_PAGE:
		break;
	case CAMERA_PAGE:
		break;
	case CONTROL_VERSION_PAGE:
		break;
	case IMAGE_PAGE:
		break;
	case VIDEO_PAGE:
		break;
	default:
		break;
	}
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
