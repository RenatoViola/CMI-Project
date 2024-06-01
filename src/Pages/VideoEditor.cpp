#include "Pages/VideoEditor.h"

void VideoEditor::setup(VideoMedia* vid) {
    // Assign the provided ImageMedia object to the class member
    video = vid;
//    frame.allocate(vid->getWidth(), vid->getHeight(), OF_IMAGE_COLOR);

    gui.setup();

    homeBtn.setup("homeIcon.png", 100, 50, 50);
    versionBtn.setup("versionIcon.png", 100, 50, 200);

    ofAddListener(homeBtn.clickedInside, this, &VideoEditor::gotoHomePage);
    ofAddListener(versionBtn.clickedInside, this, &VideoEditor::gotoVersionPage);
}

void VideoEditor::update() {
    // Here you would typically handle real-time adjustments to the image
    // For example, applying brightness or contrast settings
    video->update();

    /*
    if (video->getContent().isFrameNew()) {
        frame = video->getPixels();
    }
    */
    
    gui.update();
}

void VideoEditor::draw() {
    video->play();

    video->drawInFullscreen(ofColor::black);
//    Media::drawInFullscreen(frame, ofColor::black);
    homeBtn.draw();
    versionBtn.draw();
    gui.draw(); // Draw the GUI on top of the image
}

void VideoEditor::mouseReleased(int x, int y, int button) {
    homeBtn.mouseReleased(x, y, button);
    versionBtn.mouseReleased(x, y, button);
}

void VideoEditor::gotoHomePage() {
    int PAGE = MAIN_PAGE;
    ofNotifyEvent(redirectEvent, PAGE, this);
}

void VideoEditor::gotoVersionPage() {
    int PAGE = CONTROL_VERSION_PAGE;
    ofNotifyEvent(redirectEvent, PAGE, this);
}


void VideoEditor::exit() {
    video->exit();
    ofRemoveListener(homeBtn.clickedInside, this, &VideoEditor::gotoHomePage);
    ofRemoveListener(versionBtn.clickedInside, this, &VideoEditor::gotoVersionPage);
}