#pragma once
#include "ofMain.h"
#include "FilterPanel.h"
#include "Media.h"
#include "Metadata.h"
#include "ofApp.h"

class MediaEditor
{
public:

	virtual void setup(Media* media) {
		this->media = media;

		media->setFullScreenSizeAndPos(&displayWidth, &displayHeight, &xPos, &yPos);
		
		colorImg.allocate(media->getWidth(), media->getHeight());
		colorImg.setFromPixels(media->getPixels());

		homeBtn.setup("homeIcon.png", 100, 50, 50);
		versionBtn.setup("versionIcon.png", 100, 50, 200);

		ofAddListener(homeBtn.clickedInside, this, &MediaEditor::gotoHomePage);
		ofAddListener(versionBtn.clickedInside, this, &MediaEditor::gotoVersionPage);
	}

	void exit() {
		media->exit();
		ofRemoveListener(homeBtn.clickedInside, this, &MediaEditor::gotoHomePage);
		ofRemoveListener(versionBtn.clickedInside, this, &MediaEditor::gotoVersionPage);
	}

	virtual void draw() = 0;

	void mouseReleased(int x, int y, int button) {
		homeBtn.mouseReleased(x, y, button);
		versionBtn.mouseReleased(x, y, button);
	}

	void gotoHomePage() {
		int PAGE = MAIN_PAGE;
		ofNotifyEvent(redirectEvent, PAGE, this);
	}

	void gotoVersionPage() {
		int PAGE = CONTROL_VERSION_PAGE;
		ofNotifyEvent(redirectEvent, PAGE, this);
	}


	void drawEdges() {
		for (int i = 0; i < contourFinder.nBlobs; i++) {
			ofPolyline polyline;
			for (int j = 0; j < contourFinder.blobs[i].nPts; j++) {
				ofPoint point = contourFinder.blobs[i].pts[j];
				point.x += xPos;
				point.y += yPos;
				polyline.addVertex(point);
			}
			polyline.draw();
		}
	}

	void findContours() {
		ofxCvGrayscaleImage grayImg, binaryImg;
		grayImg.allocate(colorImg.getWidth(), colorImg.getHeight());
		grayImg = colorImg;
		grayImg.resize(displayWidth, displayHeight);

		cv::Mat grayMat = cv::cvarrToMat(grayImg.getCvImage());
		cv::Mat binaryMat;
		threshold(grayMat, binaryMat, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

		binaryImg.allocate(grayImg.width, grayImg.height);
		binaryImg.setFromPixels(binaryMat.data, grayImg.width, grayImg.height);

		contourFinder.findContours(binaryImg, 20, (displayWidth * displayHeight) / 2, 10, false);
	}

	Media* media;
	FilterPanel* gui;

	ofEvent<int> redirectEvent;
	ofxCvColorImage colorImg;
	Button homeBtn, versionBtn;
	ofxCvContourFinder contourFinder;
	float displayWidth, displayHeight, xPos, yPos;
};

