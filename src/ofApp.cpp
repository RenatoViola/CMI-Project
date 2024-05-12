#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(255, 255, 255);
	ofSetVerticalSync(true);

	// setup Screen Saver Page
	screenSaverPage.setup();

	// Uncomment this to show movies with alpha channels
	// videoPlayer.setPixelFormat(OF_PIXELS_RGBA);

	///////////////////////////////////////////////////////////
	vector<unique_ptr<Media>> imagesMedia;

	ofDirectory imgDir;
	imgDir.allowExt("jpg");
	imgDir.allowExt("png");
	imgDir.listDir("images/");

	imagesMedia.reserve(imgDir.size());

	for (int i = 0; i < (int)imgDir.size(); i++) {
		auto image = make_unique<ImageMedia>();
		image->load(imgDir.getPath(i));
		imagesMedia.push_back(move(image));
	}

	imageMediaCarrousel.setup(move(imagesMedia), true, "IMAGES");


	vector<unique_ptr<Media>> videosMedia;

	ofDirectory vidDir;
	vidDir.allowExt("mp4");
	vidDir.listDir("videos/");

	videosMedia.reserve(vidDir.size());

	for (int i = 0; i < (int)vidDir.size(); i++) {
		auto video = make_unique<VideoMedia>();
		video->load(vidDir.getPath(i));
		videosMedia.push_back(move(video));
	}

	videoMediaCarrousel.setup(move(videosMedia), false, "VIDEOS");


	///////////////////////////////////////////////////////////

	
	//imageCarrousel.setup("images/");
	//videoCarrousel.setup("videos/");
	
	
	videoGrabber.setup(1280, 720);

	

	openedImage = false;
	openedVideo = false;
	detectionEnabled = false;
	
	activePage = SCREEN_SAVER_PAGE;

	ofSetVerticalSync(true);

	//ofAddListener(imageCarrousel.onChangeScreen, this, &ofApp::changeScreen);
	//ofAddListener(videoCarrousel.onChangeScreen, this, &ofApp::changeScreen);

	ofAddListener(imageMediaCarrousel.onChangeScreen, this, &ofApp::changeScreen);
	ofAddListener(videoMediaCarrousel.onChangeScreen, this, &ofApp::changeScreen);
}

//--------------------------------------------------------------
void ofApp::update() {

	switch (activePage)
	{
	case SCREEN_SAVER_PAGE:
		screenSaverPage.update();
		break;
	case MAIN_PAGE:
	//	videoCarrousel.update();
		imageMediaCarrousel.update();
		videoMediaCarrousel.update();
		break;
	case FILTERED_PAGE:
		break;
	case CAMERA_PAGE:
		videoGrabber.update(detectionEnabled);
		break;
	case CONTROL_VERSION_PAGE:
		break;
	default:
		break;
	}


	if (activePage != CAMERA_PAGE)
	{
		detectionEnabled = false;
	}
}

void ofApp::draw() {

	switch (activePage)
	{
	case SCREEN_SAVER_PAGE:
		screenSaverPage.draw();
		break;
	case MAIN_PAGE:
		imageMediaCarrousel.draw();
		videoMediaCarrousel.draw();
		break;
	case FILTERED_PAGE:
		break;
	case CAMERA_PAGE:
		videoGrabber.drawCamera(detectionEnabled);
		break;
	case CONTROL_VERSION_PAGE:
		break;
	case IMAGE_PAGE:
	//	imageCarrousel.displayCurrent();
		imageMediaCarrousel.getCurrentMedia()->drawInFullscreen();
		break;
	case VIDEO_PAGE:
	//	Media* selectedMedia = videoMediaCarrousel.getCurrentMedia();
		videoMediaCarrousel.getCurrentMedia()->drawInFullscreen();
		
		//video->play();
	//	videoCarrousel.displayCurrent();
		break;
	default:
		break;
	}
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key) {
	case 'p':
	//	(dynamic_cast<VideoMedia*>(videoMediaCarrousel.getCurrentMedia()))->pause();
		break;
	case 'f':
		detectionEnabled = !detectionEnabled;
		break;
	case 'm':
		if (activePage == MAIN_PAGE)
			activePage = VIDEO_PAGE;
		else if (activePage == VIDEO_PAGE)
			activePage = MAIN_PAGE;

		//if(!openedImage) openedVideo = !openedVideo;
		break;
	case 'k':
		if (activePage == MAIN_PAGE)
			activePage = IMAGE_PAGE;
		else if (activePage == IMAGE_PAGE)
			activePage = MAIN_PAGE;
		//if(!openedVideo) openedImage = !openedImage;
		break;
	case 'c':
		activePage = CAMERA_PAGE;
		break;
	case OF_KEY_RETURN:
		if (activePage == SCREEN_SAVER_PAGE)
			activePage = MAIN_PAGE;
		break;
	}
}

void ofApp::changeScreen(int& page) {
	switch (activePage)
	{
		case SCREEN_SAVER_PAGE:
		//	screenSaverPage.draw();
			break;
		case MAIN_PAGE:
			imageMediaCarrousel.exit();
			videoMediaCarrousel.exit();
			break;
		case FILTERED_PAGE:
			break;
		case CAMERA_PAGE:
		//	videoGrabber.drawCamera(detectionEnabled);
			break;
		case CONTROL_VERSION_PAGE:
			break;
		case IMAGE_PAGE:
		//	imageCarrousel.displayCurrent();
			break;
		case VIDEO_PAGE:
		//	videoCarrousel.displayCurrent();
			break;
		default:
			break;
	}

	activePage = page;

	switch (activePage)
	{
	case SCREEN_SAVER_PAGE:
		screenSaverPage.setup();
		break;
	case MAIN_PAGE:
		break;
	case FILTERED_PAGE:
		break;
	case CAMERA_PAGE:
		videoGrabber.setup(1280, 720);
		break;
	case CONTROL_VERSION_PAGE:
		break;
	case IMAGE_PAGE:
		break;
	case VIDEO_PAGE:
	//	(dynamic_cast<VideoMedia*>(videoMediaCarrousel.getCurrentMedia()))->play();
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
