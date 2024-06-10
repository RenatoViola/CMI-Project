#include "FilteredPage.h"

void FilteredPage::setup(ofPixels& frame)
{
	/* Camera Vision Properties */
	vidGrabber.setDeviceID(0);
	vidGrabber.setDesiredFrameRate(30);
	vidGrabber.setVerbose(true);
	vidGrabber.setup(CAMERA_WIDTH, CAMERA_HEIGHT);

	colorImg.allocate(CAMERA_WIDTH, CAMERA_HEIGHT);
	grayImg.allocate(CAMERA_WIDTH, CAMERA_HEIGHT);

	finder.setup("aGest.xml");

	ofDirectory imgDir;
	imgDir.allowExt("jpg");
	imgDir.listDir("images/");

	ofDirectory vidDir;
	vidDir.allowExt("mp4");
	vidDir.listDir("videos/");

	vector<string> img_paths, vid_paths;
	img_paths.reserve(imgDir.size());
	vid_paths.reserve(vidDir.size());

	for (int i = 0; i < imgDir.size(); i++) {
		img_paths.push_back(imgDir.getPath(i));
	}

	for (int i = 0; i < vidDir.size(); i++) {
		vid_paths.push_back(vidDir.getPath(i));
	}

	vector<string> matching_paths = Metadata::filesWithObject(frame, img_paths, vid_paths);

	homeBtn.setup("icons/homeIcon.png", 100, 50, 50);
	mediaCir.setup(matching_paths, 350, DISPLAY_CAMERA_WIDTH, DISPLAY_CAMERA_HEIGHT);

	Media::setFullScreenSizeAndPos(DISPLAY_CAMERA_WIDTH, DISPLAY_CAMERA_HEIGHT, &displayWidth, &displayHeight, &xPos, &yPos);
	scaleX = displayWidth / CAMERA_WIDTH;
	scaleY = displayHeight / CAMERA_HEIGHT;

	// Setup the grid areas
	setupGridAreas();

	ofAddListener(homeBtn.clickedInside, this, &FilteredPage::gotoHomePage);
	ofAddListener(mediaCir.clickedOnItem, this, &FilteredPage::gotoFilePage);
}


void FilteredPage::draw()
{
	// Draw the resized image
	colorImg.draw(xPos, yPos, displayWidth, displayHeight);

	// Draw the grid for debugging purposes
	ofNoFill();
	ofSetColor(ofColor::blue);
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			ofDrawRectangle(gridAreas[i][j]);
		}
	}

	// Draw the bounding boxes for blobs
	ofNoFill();
	ofSetColor(ofColor::red);
	for (int i = 0; i < finder.blobs.size(); i++) {
		ofRectangle cur = finder.blobs[i].boundingRect;

		// Normalize the positions and sizes
		float r_xPos = cur.x * scaleX;
		float r_yPos = cur.y * scaleY;
		float r_displayWidth = cur.getWidth() * scaleX;
		float r_displayHeight = cur.getHeight() * scaleY;

		// Draw the rectangles at the correct position
		ofDrawRectangle(r_xPos + xPos, r_yPos + yPos, r_displayWidth, r_displayHeight);
	}
	ofFill();
	ofSetColor(ofColor::white);

	// Draw the other UI elements
	mediaCir.draw();
	homeBtn.draw();
}


void FilteredPage::update()
{
	vidGrabber.update();

	if (vidGrabber.isFrameNew()) {
		colorImg.setFromPixels(vidGrabber.getPixels());
		colorImg.mirror(false, true);
		grayImg = colorImg;
		finder.findHaarObjects(grayImg);
	}

	filterBlobs(20000);
	checkBlobs();

	mediaCir.update();
}

void FilteredPage::gotoHomePage()
{
	int PAGE = MAIN_PAGE;
	ofNotifyEvent(redirectEvent, PAGE, this);
}

void FilteredPage::gotoFilePage()
{
	selectedFilePath = mediaCir.getCurrentFilePath();

	int PAGE;
	if (Media::isImage(selectedFilePath))
	{
		PAGE = IMAGE_PAGE;
	}
	else
	{
		PAGE = VIDEO_PAGE;
	}

	ofNotifyEvent(redirectEvent, PAGE, this);
}

void FilteredPage::mouseReleased(int x, int y, int button) {
	homeBtn.mouseReleased(x, y, button);
	mediaCir.mouseReleased(x, y, button);
}

string FilteredPage::getCurrentFilePath() {
	return selectedFilePath;
}


void FilteredPage::setupGridAreas() {
	float cellWidth = displayWidth / 3;
	float cellHeight = displayHeight / 3;

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			gridAreas[i][j] = ofRectangle(xPos + j * cellWidth, yPos + i * cellHeight, cellWidth, cellHeight);
		}
	}
}

void FilteredPage::checkBlobs() {
        for (int i = 0; i < finder.blobs.size(); i++) {
            ofRectangle cur = finder.blobs[i].boundingRect;

            float r_xPos = cur.x * scaleX;
            float r_yPos = cur.y * scaleY;
            ofPoint normalizedPos(r_xPos + xPos, r_yPos + yPos);

            // Check non-diagonal and non-middle areas
            if (gridAreas[0][1].inside(normalizedPos)) {
				ofLogError() << "UP" << endl; // Top middle area
            } else if (gridAreas[1][0].inside(normalizedPos)) {
				ofLogError() << "LEFT" << endl; // Middle left area
            } else if (gridAreas[1][2].inside(normalizedPos)) {
				ofLogError() << "RIGHT" << endl;; // Middle right area
            } else if (gridAreas[2][1].inside(normalizedPos)) {
				ofLogError() << "BOTTOM" << endl; // Bottom middle area
            }
        }
}

void FilteredPage::filterBlobs(float minBlobSize) {
	std::vector<ofxCvBlob> filteredBlobs;
	for (int i = 0; i < finder.blobs.size(); i++) {
		if (finder.blobs[i].boundingRect.getArea() > minBlobSize) {
			filteredBlobs.push_back(finder.blobs[i]);
		}
	}
	finder.blobs = filteredBlobs;
}

void FilteredPage::exit() {
	mediaCir.exit();
	colorImg.clear();
	grayImg.clear();
	vidGrabber.close();

	ofRemoveListener(homeBtn.clickedInside, this, &FilteredPage::gotoHomePage);
	ofRemoveListener(mediaCir.clickedOnItem, this, &FilteredPage::gotoFilePage);

	
}