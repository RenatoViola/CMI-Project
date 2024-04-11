#include "VideoGrabber.h"

//--------------------------------------------------------------
void VideoGrabber::setup(int width, int height) {
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
	vidGrabber.setup(camWidth, camHeight);
}

void VideoGrabber::update() {
	vidGrabber.update();
}

//--------------------------------------------------------------
void VideoGrabber::drawCamera() {
	ofSetColor(ofColor::black);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	ofSetHexColor(0xffffff);
	vidGrabber.draw(ofGetWidth() / 2 - camWidth/2, ofGetHeight() / 2 - camHeight/2);
}