#include "CameraPage.h"
#include <screen_names.h>

//--------------------------------------------------------------
void CameraPage::setup(int width, int height) {
	camWidth = width;  // try to grab at this size.
	camHeight = height;

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

	finder.setup("haarcascade_frontalface_default.xml");
	// finder.setScaleHaar(1.5);

	homeBtn.setup("homeIcon.png", 100, 50, 50);
	ofAddListener(homeBtn.clickedInside, this, &CameraPage::gotoPreviousPage);
}

void CameraPage::update(bool detectionEnabled) {
	vidGrabber.update();

	if (vidGrabber.isFrameNew()) {
		colorImg.setFromPixels(vidGrabber.getPixels());

		if (detectionEnabled)
		{
			grayImg = colorImg;
			finder.findHaarObjects(grayImg);
		}
	}
}

//--------------------------------------------------------------
void CameraPage::drawCamera(bool detectionEnabled) {
	ofSetColor(ofColor::white);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	colorImg.draw(ofGetWidth() / 2 - camWidth / 2, ofGetHeight() / 2 - camHeight / 2);
	homeBtn.draw();

	if (detectionEnabled)
	{
		ofNoFill();
		ofSetColor(ofColor::blueSteel);
		for (int i = 0; i < finder.blobs.size(); i++) {
			ofDrawRectangle(finder.blobs[i].boundingRect);
		}
		ofFill();
	}
}

void CameraPage::mouseReleased(int x, int y, int button) {
	homeBtn.mouseReleased(x, y, button);
}

void CameraPage::gotoPreviousPage() {
	int PAGE = MAIN_PAGE;
	ofNotifyEvent(redirectEvent, PAGE, this);
}

void CameraPage::exit() {
	vidGrabber.close();
	colorImg.clear();
	grayImg.clear();
	faceRects.clear();
	ofRemoveListener(homeBtn.clickedInside, this, &CameraPage::gotoPreviousPage);
}