#pragma once
#include "ofMain.h"
#include "ofApp.h"
#include "UIElements/FilterPanel.h"
#include "DataTypes/Media.h"
#include <ofxOpenCv.h>

class MediaEditor
{
public:

	void setup(const string& filePath, int versionID) {

		this->filePath = filePath;
		homeBtn.setup("icons/homeIcon.png", 100, 50, 50);
		versionBtn.setup("icons/versionIcon.png", 100, 50, 200);
		gui.setup(filePath);
		if (versionID != 0)
		{
			gui.loadVersionInfo(versionID);
		}
		
		ofAddListener(homeBtn.clickedInside, this, &MediaEditor::gotoHomePage);
		ofAddListener(versionBtn.clickedInside, this, &MediaEditor::gotoVersionPage);
	}


	virtual void draw() = 0;


	string getFilePath() {
		return filePath;
	}


	void exit() {
		gui.exit();
		ofRemoveListener(homeBtn.clickedInside, this, &MediaEditor::gotoHomePage);
		ofRemoveListener(versionBtn.clickedInside, this, &MediaEditor::gotoVersionPage);
	}


	void mouseReleased(int x, int y, int button) {
		homeBtn.mouseReleased(x, y, button);
		versionBtn.mouseReleased(x, y, button);
	}


	ofEvent<int> redirectEvent;


protected:	

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


	float displayWidth, displayHeight, xPos, yPos;
	FilterPanel gui;
	ofxCvColorImage colorImg;
	Button homeBtn, versionBtn;


private:
	void gotoHomePage() {
		int PAGE = MAIN_PAGE;
		ofNotifyEvent(redirectEvent, PAGE, this);
	}


	void gotoVersionPage() {
		int PAGE = CONTROL_VERSION_PAGE;
		ofNotifyEvent(redirectEvent, PAGE, this);
	}


	string filePath;
	ofxCvContourFinder contourFinder;
};

