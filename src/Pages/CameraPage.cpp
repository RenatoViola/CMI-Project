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

//	finder.setup("haarcascade_frontalface_default.xml");
//	finder.setup("haarcascade_frontalface_alt.xml");
//	finder.setup("haarcascade_frontalface_alt2.xml");
	finder.setup("haarcascade_frontalface_alt_tree.xml");
	// finder.setScaleHaar(1.5);

	homeBtn.setup("homeIcon.png", 100, 50, 50);
	ofAddListener(homeBtn.clickedInside, this, &CameraPage::gotoHomePage);
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

    // Calculate the position where the image will be drawn
    float drawPosX = ofGetWidth() / 2 - camWidth / 2;
    float drawPosY = ofGetHeight() / 2 - camHeight / 2;

    // Draw the video image
    colorImg.draw(drawPosX, drawPosY);
    homeBtn.draw();

    if (detectionEnabled) {
        ofNoFill();
        ofSetColor(ofColor::steelBlue);
        for (int i = 0; i < finder.blobs.size(); i++) {
            ofRectangle cur = finder.blobs[i].boundingRect;

            // Adjust the rectangle position by the draw position offset
            ofDrawRectangle(cur.x + drawPosX, cur.y + drawPosY, cur.width, cur.height);
        }
        ofFill();
    }
}


void CameraPage::mouseReleased(int x, int y, int button) {
	homeBtn.mouseReleased(x, y, button);
}

void CameraPage::gotoHomePage() {
	int PAGE = MAIN_PAGE;
	ofNotifyEvent(redirectEvent, PAGE, this);
}

void CameraPage::exit() {
	vidGrabber.close();
	colorImg.clear();
	grayImg.clear();
	faceRects.clear();
	ofRemoveListener(homeBtn.clickedInside, this, &CameraPage::gotoHomePage);
}