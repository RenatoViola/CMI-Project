#include "ScreenSaver.h"
#include <DataTypes/Media.h>
#include <screen_names.h>

void ScreenSaver::setup()
{
	bLearnBackground = true;
	changeScreen = false;
	counterToLearn = 0;

	videoGrabber.setDeviceID(0);
	videoGrabber.setDesiredFrameRate(30);
	videoGrabber.setVerbose(true);
	videoGrabber.setup(CAMERA_WIDTH, CAMERA_HEIGHT);

	currentFrame.allocate(CAMERA_WIDTH, CAMERA_HEIGHT);
	bgImage.allocate(CAMERA_WIDTH, CAMERA_HEIGHT);

	img.load(PATH);
	colorImg.allocate(img.getWidth(), img.getHeight());
	colorImg.setFromPixels(img.getPixels());
}

void ScreenSaver::draw() 
{
	#ifdef CAMERA_DEBUG
	
		ofSetColor(ofColor::black);
		ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

		ofSetHexColor(0xffffff);
		diff.draw(CAMERA_WIDTH, CAMERA_HEIGHT);

	#else
		float displayWidth, displayHeight, xPos, yPos;
		Media::setFullScreenSizeAndPos(colorImg, &displayWidth, &displayHeight, &xPos, &yPos);
		colorImg.draw(xPos, yPos, displayWidth, displayHeight);
	
	#endif // CAMERA_DEBUG

}

void ScreenSaver::update()
{
	if (changeScreen) return;

	videoGrabber.update();

	if (videoGrabber.isFrameNew()) {

		ofxCvColorImage color;
		color.setFromPixels(videoGrabber.getPixels());
		currentFrame = color;

		if (bLearnBackground) {
			bLearnBackground = false;
			bgImage = currentFrame;
		}

		diff.absDiff(bgImage,currentFrame);
		diff.threshold(THRESHOLD); 


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
		checkForMovement();
	}
}

void ScreenSaver::exit() {
	videoGrabber.close();
	currentFrame.clear();
	img.clear();
	bgImage.clear();
	colorImg.clear();
}

void ScreenSaver::checkForMovement()
{
	int movementThreshold = 10000; // Adjust this threshold as needed
	int whitePixelCount = 0;

	ofPixels pixels = diff.getPixels();
	// Iterate through the image pixels
	for (int y = 0; y < CAMERA_HEIGHT; y++) {
		for (int x = 0; x < CAMERA_WIDTH; x++) {
			ofColor pixelColor = pixels.getColor(x + y);
			// Check if the pixel is white (movement detected)
			if (pixelColor.getBrightness() > 0) {
				whitePixelCount++;
			}
		}
	}

	// Verify if movement exceeds the threshold
	if (whitePixelCount > movementThreshold) {
		int PAGE = MAIN_PAGE;
		changeScreen = true;
		cout << "Movement detected." << endl;
		ofNotifyEvent(redirectEvent, PAGE, this);
	}
}