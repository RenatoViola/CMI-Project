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

	//vidGrabber.setDeviceID(0);
	//vidGrabber.setDesiredFrameRate(30);

	vidGrabber.setVerbose(true);
	vidGrabber.setup(width, height);

	colorImg.allocate(width, height);
	grayImg.allocate(width, height);

	finder.setup("haarcascade_frontalface_default.xml");
	// finder.setScaleHaar(1.5);
}

void VideoGrabber::update(bool detectionEnabled) {
	vidGrabber.update();

	if (vidGrabber.isFrameNew()) {
		colorImg.setFromPixels(vidGrabber.getPixels());
	//	colorImg.invert();

		if (detectionEnabled)
		{
			grayImg = colorImg;
			finder.findHaarObjects(grayImg);
		}
	}

}

//--------------------------------------------------------------
void VideoGrabber::drawCamera(bool detectionEnabled) {
	ofSetColor(ofColor::black);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	ofSetColor(ofColor::white);
	colorImg.draw(ofGetWidth() / 2 - camWidth / 2, ofGetHeight() / 2 - camHeight / 2);

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