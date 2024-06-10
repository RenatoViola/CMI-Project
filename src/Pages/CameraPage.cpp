#include "CameraPage.h"
#include <screen_names.h>

//--------------------------------------------------------------
void CameraPage::setup(int width, int height) {
	camWidth = width;  // try to grab at this size.
	camHeight = height;

	facialDetectionEnabled = false;

	//get back a list of devices.
	vector<ofVideoDevice> devices = vidGrabber.listDevices();

	for (size_t i = 0; i < devices.size(); i++) {
		if (devices[i].bAvailable) {
			//log the device
			ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
		}
		else {
			//log the device and note it as unavailable
			ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
		}
	}

	vidGrabber.setDeviceID(0);
	vidGrabber.setDesiredFrameRate(30);

	vidGrabber.setVerbose(true);
	vidGrabber.setup(width, height);

	colorImg.allocate(width, height);
	grayImg.allocate(width, height);

	finder.setup("haarcascade_frontalface_alt_tree.xml");
	myFont.load("JuliusSansOne-Regular.ttf", 32);

	homeBtn.setup("icons/homeIcon.png", 100, 50, 50);
	searchBtn.setup("icons/searchIcon.png", 100, 50, 200);
	faceBtn.setup("icons/faceIcon.png", 100, 200, 50);

	ofAddListener(homeBtn.clickedInside, this, &CameraPage::gotoHomePage);
	ofAddListener(searchBtn.clickedInside, this, &CameraPage::initiateSearchResultPage);
	ofAddListener(faceBtn.clickedInside, this, &CameraPage::toggleFaceDetection);
}

void CameraPage::update() {
	vidGrabber.update();

	if (vidGrabber.isFrameNew()) {
		colorImg.setFromPixels(vidGrabber.getPixels());

		if (facialDetectionEnabled)
		{
			grayImg = colorImg;
			finder.findHaarObjects(grayImg);
		}
	}

	// Handle the redirection with a delay
	if (isRedirecting && (ofGetElapsedTimeMillis() - startTime) > 500) {
		gotoSearchResultPage();
		isRedirecting = false;
	}
}

//--------------------------------------------------------------
void CameraPage::drawCamera() {
    ofSetColor(ofColor::white);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

    float drawPosX = ofGetWidth() / 2 - camWidth / 2;
    float drawPosY = ofGetHeight() / 2 - camHeight / 2;

    colorImg.draw(drawPosX, drawPosY);
    homeBtn.draw();
	searchBtn.draw();
	faceBtn.draw();

    if (facialDetectionEnabled) {
        ofNoFill();
        ofSetColor(ofColor::red);
        for (int i = 0; i < finder.blobs.size(); i++) {
            ofRectangle cur = finder.blobs[i].boundingRect;
            ofDrawRectangle(cur.x + drawPosX, cur.y + drawPosY, cur.width, cur.height);
        }
        ofFill();
    }

	if (showLoadingMessage) {
		ofSetColor(ofColor::black);
		string loadingMessage = "LOADING MATCHES... THIS MIGHT TAKE A BIT :)";
		float textWidth = myFont.stringWidth(loadingMessage);
		myFont.drawString(loadingMessage, drawPosX + ((camWidth - textWidth) / 2.0f), drawPosY + camHeight + 50);
		ofSetColor(ofColor::white);
	}
}


void CameraPage::mouseReleased(int x, int y, int button) {
	homeBtn.mouseReleased(x, y, button);
	searchBtn.mouseReleased(x, y, button);
	faceBtn.mouseReleased(x, y, button);
}

void CameraPage::gotoHomePage() {
	int PAGE = MAIN_PAGE;
	ofNotifyEvent(redirectEvent, PAGE, this);
}

void CameraPage::initiateSearchResultPage() {
	capturedFrame = colorImg.getPixels();
	showLoadingMessage = true;

	// Record the start time and set the redirection flag
	startTime = ofGetElapsedTimeMillis();
	isRedirecting = true;
}

void CameraPage::gotoSearchResultPage() {
	int PAGE = FILTERED_PAGE;
	ofNotifyEvent(redirectEvent, PAGE, this);
}

void CameraPage::toggleFaceDetection() {
	facialDetectionEnabled = !facialDetectionEnabled;
}

ofPixels CameraPage::getCapturedFrame() const {
	return capturedFrame;
}

void CameraPage::exit() {
	vidGrabber.close();
	colorImg.clear();
	grayImg.clear();
	faceRects.clear();
	showLoadingMessage = false;
	isRedirecting = false;

	ofRemoveListener(homeBtn.clickedInside, this, &CameraPage::gotoHomePage);
	ofRemoveListener(searchBtn.clickedInside, this, &CameraPage::gotoSearchResultPage);
	ofRemoveListener(faceBtn.clickedInside, this, &CameraPage::toggleFaceDetection);
}