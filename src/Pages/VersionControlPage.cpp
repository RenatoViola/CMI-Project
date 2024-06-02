#include "VersionControlPage.h"

#include <DataTypes/Media.h>
#include <screen_names.h>


void VersionControlPage::setup()
{

	ofAddListener(homeBtn.clickedInside, this, &VersionControlPage::gotoHomePage);

	/* Camera Vision Properties */
	bLearnBackground = true;

	videoGrabber.setDeviceID(0);
	videoGrabber.setDesiredFrameRate(30);
	videoGrabber.setVerbose(true);
	videoGrabber.setup(CAMERA_WIDHT, CAMERA_HEIGHT);

	currentFrame.allocate(CAMERA_WIDHT, CAMERA_HEIGHT);
	bgImage.allocate(CAMERA_WIDHT, CAMERA_HEIGHT);

	// TODO - initialize media circle
}

void VersionControlPage::draw()
{
#ifdef CAMERA_DEBUG

	ofSetColor(ofColor::black);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	ofSetHexColor(0xffffff);
	diff.draw(CAMERA_WIDHT, CAMERA_HEIGHT);

	contourFinder.draw(CAMERA_WIDHT, CAMERA_HEIGHT);

#else

	// TODO - draw media circle

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
}

void VersionControlPage::exit() {
	videoGrabber.close();
	currentFrame.clear();
	diff.clear();
	bgImage.clear();
}

void VersionControlPage::checkForMovement()
{

	#pragma region Find countours blobs

	int maxArea = (CAMERA_HEIGHT * CAMERA_WIDHT) / 3;

	contourFinder.findContours(diff, 20, maxArea, 10, false);

	#pragma endregion

}

void VersionControlPage::gotoHomePage()
{
	int PAGE = MAIN_PAGE;
	ofNotifyEvent(redirectEvent, PAGE, this);
}