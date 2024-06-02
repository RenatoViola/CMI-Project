#include "VersionControlPage.h"

void VersionControlPage::setup()
{
	homeBtn.setup("homeIcon.png", 100, 50, 50);
	ofAddListener(homeBtn.clickedInside, this, &VersionControlPage::gotoHomePage);

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
	ofDirectory videoDir;
	vector<unique_ptr<Media>> videos;
	videoDir.listDir("videos/");

	videos.reserve(8);

	for (int i = 0; i < 8; i++) {
		auto video = make_unique<VideoMedia>();
		video->load(videoDir.getPath(i));
		videos.push_back(move(video));
	}
	mediaCir.setup(move(videos));
}

void VersionControlPage::draw()
{
#ifdef CAMERA_DEBUG

	float displayWidth, displayHeight, xPos, yPos;
	Media::setFullScreenSizeAndPos(CAMERA_WIDTH, CAMERA_HEIGHT, &displayWidth, &displayHeight, &xPos, &yPos);

	ofSetHexColor(0xffffff);
	diff.draw(xPos, yPos, displayWidth, displayHeight);
	contourFinder.draw(xPos, yPos, displayWidth, displayHeight);

	mediaCir.draw();
#else

	// TODO - draw media circle
	mediaCir.draw();

#endif // CAMERA_DEBUG

	homeBtn.draw();
}

void VersionControlPage::update()
{
	videoGrabber.update();

	if (videoGrabber.isFrameNew()) {
		
		ofxCvColorImage color;
		color.setFromPixels(videoGrabber.getPixels());
		color.mirror(false,true); // flip image horizontaly
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

void VersionControlPage::exit() {
	mediaCir.exit();
	videoGrabber.close();
	currentFrame.clear();
	diff.clear();
	bgImage.clear();
}

void VersionControlPage::checkForMovement()
{

	#pragma region Find countours blobs

	int maxArea = (CAMERA_HEIGHT * CAMERA_WIDTH) / 3;

	contourFinder.findContours(diff, 20, maxArea, 10, false);

	#pragma endregion

}

void VersionControlPage::gotoHomePage()
{
	int PAGE = MAIN_PAGE;
	ofNotifyEvent(redirectEvent, PAGE, this);
}

void VersionControlPage::mouseReleased(int x, int y, int button) {
	homeBtn.mouseReleased(x, y, button);
}
