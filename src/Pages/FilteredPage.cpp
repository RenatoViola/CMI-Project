#include "FilteredPage.h"

void FilteredPage::setup(ofPixels& frame)
{
	/* Camera Vision Properties */
	bLearnBackground = true;

	videoGrabber.setDeviceID(0);
	videoGrabber.setDesiredFrameRate(30);
	videoGrabber.setVerbose(true);
	videoGrabber.setup(CAMERA_WIDTH, CAMERA_HEIGHT);

	currentFrame.allocate(CAMERA_WIDTH, CAMERA_HEIGHT);
	bgImage.allocate(CAMERA_WIDTH, CAMERA_HEIGHT);

	// TODO - initialize media circle / find and load media files
	/* Initialize media circle */

	// Test object recognition
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
	mediaCir.setup(matching_paths, 350, CAMERA_WIDTH, CAMERA_HEIGHT);

	ofAddListener(homeBtn.clickedInside, this, &FilteredPage::gotoHomePage);
	ofAddListener(mediaCir.clickedOnItem, this, &FilteredPage::gotoFilePage);

	img.allocate(frame.getWidth(), frame.getHeight());
	img.setFromPixels(frame);
}


void FilteredPage::draw()
{
	float displayWidth, displayHeight, xPos, yPos;
	Media::setFullScreenSizeAndPos(CAMERA_WIDTH, CAMERA_HEIGHT, &displayWidth, &displayHeight, &xPos, &yPos);

	//ofSetHexColor(0xffffff);
	//diff.draw(xPos, yPos, displayWidth, displayHeight);
	//contourFinder.draw(xPos, yPos, displayWidth, displayHeight);

	img.draw(xPos, yPos, displayWidth, displayHeight);

	mediaCir.draw();
	homeBtn.draw();
}


void FilteredPage::update()
{
	videoGrabber.update();

	if (videoGrabber.isFrameNew()) {

		ofxCvColorImage color;
		color.setFromPixels(videoGrabber.getPixels());
		color.mirror(false, true); // flip image horizontaly
		currentFrame = color;

#pragma region Learn Background

		if (bLearnBackground) {
			bLearnBackground = false;
			bgImage = currentFrame;
		}

#pragma endregion

		diff.absDiff(bgImage, currentFrame);
		diff.threshold(THRESHOLD);

#pragma region Background subtraction over time

		// Convert ofxCvGrayscaleImage to cv::Mat
		cv::Mat frame = cv::cvarrToMat(currentFrame.getCvImage());
		cv::Mat bgMat = cv::cvarrToMat(bgImage.getCvImage());

		// Convert to float type for accumulateWeighted
		frame.convertTo(frame, CV_32F);
		bgMat.convertTo(bgMat, CV_32F);

		try {
			// Background updated over time
			cv::accumulateWeighted(frame, bgMat, ALPHA);
			bgMat.convertTo(bgMat, CV_8U);
			bgImage.setFromPixels(bgMat.data, bgMat.cols, bgMat.rows);
		}
		catch (const cv::Exception& e) {
			std::cerr << "cv::Exception: " << e.what() << std::endl;

		}

#pragma endregion

		checkForMovement();
	}

	mediaCir.update();
}


void FilteredPage::checkForMovement()
{

#pragma region Find countours blobs

	int maxArea = (CAMERA_HEIGHT * CAMERA_WIDTH) / 3;

	contourFinder.findContours(diff, 20, maxArea, 10, false);

#pragma endregion

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

void FilteredPage::exit() {
	mediaCir.exit();
	videoGrabber.close();
	currentFrame.clear();
	diff.clear();
	bgImage.clear();

	ofRemoveListener(homeBtn.clickedInside, this, &FilteredPage::gotoHomePage);
	ofRemoveListener(mediaCir.clickedOnItem, this, &FilteredPage::gotoFilePage);

	img.clear();
}