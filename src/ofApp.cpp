#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(255, 255, 255);
	ofSetVerticalSync(true);

	// LOAD METADATA
	ofDirectory imgDir, vidDir;
	imgDir.allowExt("jpg");
	imgDir.listDir("images/");
	vidDir.allowExt("mp4");
	vidDir.listDir("videos/");
	
	for (int i = 0; i < imgDir.size(); i++)
		Metadata::load(imgDir.getPath(i));

	for (int i = 0; i < vidDir.size(); i++)
		Metadata::load(vidDir.getPath(i));

	// setup Screen Saver Page
	screenSaverPage.setup();
	homePage.setup();

	// Uncomment this to show movies with alpha channels
	// videoPlayer.setPixelFormat(OF_PIXELS_RGBA);
	
	activePage = SCREEN_SAVER_PAGE;

	ofSetVerticalSync(true);

	ofAddListener(homePage.redirectEvent, this, &ofApp::changeScreen);
	ofAddListener(screenSaverPage.redirectEvent, this, &ofApp::changeScreen);
	ofAddListener(imagePage.redirectEvent, this, &ofApp::changeScreen);
	ofAddListener(videoPage.redirectEvent, this, &ofApp::changeScreen);
	ofAddListener(cameraPage.redirectEvent, this, &ofApp::changeScreen);
	ofAddListener(verCtrPage.redirectEvent, this, &ofApp::changeScreen);
	ofAddListener(filteredPage.redirectEvent, this, &ofApp::changeScreen);
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
			filteredPage.update();
			break;
		case CAMERA_PAGE:
			cameraPage.update();
			break;
		case CONTROL_VERSION_PAGE:
			verCtrPage.update();
			break;
		case IMAGE_PAGE:
			break;
		case VIDEO_PAGE:
			videoPage.update();
			break;
		default:
			break;
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
			filteredPage.draw();
			break;
		case CAMERA_PAGE:
			cameraPage.drawCamera();
			break;
		case CONTROL_VERSION_PAGE:
			verCtrPage.draw();
			break;
		case IMAGE_PAGE:
			imagePage.draw();
			break;
		case VIDEO_PAGE:
			videoPage.draw();
			break;
		default:
			break;
	}
}

void ofApp::changeScreen(int& page) {

	string selectedFilePath = "";
	int versionID = 0;

	switch (activePage)
	{
		// Close previous page
	case SCREEN_SAVER_PAGE:
		screenSaverPage.exit();
		break;
	case MAIN_PAGE:
		selectedFilePath = homePage.getCurrentFilePath();
		//	homePage.exit();
		break;
	case FILTERED_PAGE:
		{
			selectedFilePath = filteredPage.getCurrentFilePath();
			filteredPage.exit();
		}
		break;
	case CAMERA_PAGE:
		cameraPage.exit();
		break;
	case CONTROL_VERSION_PAGE:
		{
			selectedFilePath = verCtrPage.getCurrentFilePath();
			versionID = verCtrPage.getCurrentVersion();
			verCtrPage.exit();
		}
		break;
	case IMAGE_PAGE:
		selectedFilePath = imagePage.getFilePath();
		imagePage.exit();
		break;
	case VIDEO_PAGE:
		selectedFilePath = videoPage.getFilePath();
		videoPage.exit();
		break;
	default:
		break;
	}

	activePage = page;

	switch (activePage)
	{
	case SCREEN_SAVER_PAGE:
		screenSaverPage.setup();
		break;
	case MAIN_PAGE:
		//	homePage.setup();
		break;
	case FILTERED_PAGE:
		filteredPage.setup(cameraPage.getCapturedFrame());
		break;
	case CAMERA_PAGE:
		cameraPage.setup(1280, 720);
		break;
	case CONTROL_VERSION_PAGE:
		verCtrPage.setup(selectedFilePath);
		break;
	case IMAGE_PAGE:
		imagePage.setup(selectedFilePath, versionID);
		break;
	case VIDEO_PAGE:
		videoPage.setup(selectedFilePath, versionID);
		break;
	default:
		break;
	}
}

#pragma region Event listeners

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
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
			filteredPage.mouseReleased(x, y, button);
			break;
		case CAMERA_PAGE:
			cameraPage.mouseReleased(x, y, button);
			break;
		case CONTROL_VERSION_PAGE:
			verCtrPage.mouseReleased(x, y, button);
			break;
		case IMAGE_PAGE:
			imagePage.mouseReleased(x, y, button);
			break;
		case VIDEO_PAGE:
			videoPage.mouseReleased(x, y, button);
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

#pragma endregion
