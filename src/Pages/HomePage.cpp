#include "HomePage.h"

void HomePage::setup() {

    int mediaHeight = 240;
    int horizontalMiddle = (ofGetHeight() - mediaHeight) / 2;

    // Top half of the screen
    imageCarrousel.setup("images/", horizontalMiddle - mediaHeight / 2, true);

    // Bottom half of the screen
    videoCarrousel.setup("videos/", horizontalMiddle + mediaHeight, false);

    // Mouse event listeners
    ofAddListener(imageCarrousel.clickedOnSelected, this, &HomePage::gotoImagePage);
    ofAddListener(videoCarrousel.clickedOnSelected, this, &HomePage::gotoVideoPage);
    
    // Buttons and their listeners
    cameraBtn.setup("icons/cameraIcon.png", 100, 80, 50);
    lockBtn.setup("icons/lockIcon.png", 100, ofGetWidth() - 180, 50);

    ofAddListener(cameraBtn.clickedInside, this, &HomePage::gotoCameraPage);
    ofAddListener(lockBtn.clickedInside, this, &HomePage::gotoSavescreenPage);
}

void HomePage::exit() {
    imageCarrousel.exit();
    videoCarrousel.exit();

    ofRemoveListener(imageCarrousel.clickedOnSelected, this, &HomePage::gotoImagePage);
    ofRemoveListener(videoCarrousel.clickedOnSelected, this, &HomePage::gotoVideoPage);
    ofRemoveListener(cameraBtn.clickedInside, this, &HomePage::gotoCameraPage);
    ofRemoveListener(lockBtn.clickedInside, this, &HomePage::gotoSavescreenPage);
}

void HomePage::update() {
    imageCarrousel.update();
    videoCarrousel.update();
}

void HomePage::draw() {
    imageCarrousel.draw();
    videoCarrousel.draw();
    cameraBtn.draw();
    lockBtn.draw();
}

string HomePage::getCurrentFilePath() {
    return selectedFilePath;
}

void HomePage::mousePressed(int x, int y, int button) {
    imageCarrousel.mousePressed(x, y, button);
    videoCarrousel.mousePressed(x, y, button);
}

void HomePage::mouseReleased(int x, int y, int button) {
    imageCarrousel.mouseReleased(x, y, button);
    videoCarrousel.mouseReleased(x, y, button);
    cameraBtn.mouseReleased(x, y, button);
    lockBtn.mouseReleased(x, y, button);
}

void HomePage::gotoImagePage() {
    selectedFilePath = imageCarrousel.getCurrentFilePath();

    int PAGE = IMAGE_PAGE;
    ofNotifyEvent(redirectEvent, PAGE, this);
}

void HomePage::gotoVideoPage() {
    selectedFilePath = videoCarrousel.getCurrentFilePath();

    int PAGE = VIDEO_PAGE;
    ofNotifyEvent(redirectEvent, PAGE, this);
}

void HomePage::gotoCameraPage() {
    int PAGE = CAMERA_PAGE;
    ofNotifyEvent(redirectEvent, PAGE, this);
}

void HomePage::gotoSavescreenPage() {
    int PAGE = SCREEN_SAVER_PAGE;
    ofNotifyEvent(redirectEvent, PAGE, this);
}