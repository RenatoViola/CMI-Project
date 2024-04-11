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

	vidGrabber.setVerbose(true);
	vidGrabber.setup(width, camHeight);

	img.allocate(width, height, OF_IMAGE_COLOR);

	finder.setup("haarcascade_frontalface_default.xml");
}

void VideoGrabber::update(bool detectionEnabled) {
	vidGrabber.update();

	if (vidGrabber.isFrameNew()) {
		img.setFromPixels(vidGrabber.getPixels());
		if (detectionEnabled)
		{
			finder.findHaarObjects(img);
		}
	}

}

//--------------------------------------------------------------
void VideoGrabber::drawCamera(bool detectionEnabled) {
	ofSetColor(ofColor::black);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	ofSetColor(ofColor::white);
	img.draw(ofGetWidth() / 2 - camWidth / 2, ofGetHeight() / 2 - camHeight / 2);

	if (detectionEnabled)
	{
		ofNoFill();
		ofSetColor(ofColor::black);
		for (unsigned int i = 0; i < finder.blobs.size(); i++) {
			ofRectangle cur = finder.blobs[i].boundingRect;
			ofDrawRectangle(cur.x, cur.y, cur.width, cur.height);
		}
		ofFill();
	}

}