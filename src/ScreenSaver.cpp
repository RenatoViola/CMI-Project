#include "ScreenSaver.h"
#include <Media.h>
#include <screen_names.h>

void ScreenSaver::setup()
{
	bLearnBackground = true;
	changeScreen = false;
	counterToLearn = 0;


	videoGrabber.setDeviceID(0);
	videoGrabber.setDesiredFrameRate(30);
	videoGrabber.setVerbose(true);
	videoGrabber.setup(CAMERA_WIDHT, CAMERA_HEIGHT);

	currentFrame.allocate(CAMERA_WIDHT, CAMERA_HEIGHT);
	bgImage.allocate(CAMERA_WIDHT, CAMERA_HEIGHT);

	img.load(PATH);
}

void ScreenSaver::draw() 
{
	#ifdef CAMERA_DEBUG
	
		ofSetColor(ofColor::black);
		ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

		ofSetHexColor(0xffffff);
		diff.draw(CAMERA_WIDHT, CAMERA_HEIGHT);

	#else

		Media::drawInFullscreen(img.getPixels(), ofColor::white);	
	
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

		checkForMovement();
	}
	

	// Relearn background after 1000 frames
	counterToLearn++;

	if (counterToLearn > 1000) {
		counterToLearn = 0;
		bLearnBackground = true;
	}

}

void ScreenSaver::exit() {
	//backgroundImage.clear();
}

void ScreenSaver::checkForMovement()
{
	int movementThreshold = CAMERA_HEIGHT * CAMERA_WIDHT / 4; // Adjust this threshold as needed
	int whitePixelCount = 0;

	ofPixels pixels = diff.getPixels();
	// Iterate through the image pixels
	for (int y = 0; y < CAMERA_HEIGHT; y++) {
		for (int x = 0; x < CAMERA_WIDHT; x++) {
			ofColor pixelColor = pixels.getColor(x + y);
			// Check if the pixel is white (movement detected)
			if (pixelColor.getBrightness() > 0) {
				whitePixelCount++;
			}
		}
	}

	// Verify if movement exceeds the threshold
	cout << whitePixelCount << endl;
	if (whitePixelCount > movementThreshold) {
		int PAGE = MAIN_PAGE;
		changeScreen = true;
		cout << "Movement detected." << endl;
		ofNotifyEvent(redirectEvent, PAGE, this);
	}
}