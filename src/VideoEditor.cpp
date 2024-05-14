#include "VideoEditor.h"

void VideoEditor::setup(VideoMedia* vid) {
    // Assign the provided ImageMedia object to the class member
    video = vid;
//    frame.allocate(vid->getWidth(), vid->getHeight(), OF_IMAGE_COLOR);

    gui.setup();

    backBtn.setup("backIcon.png", 100, 45, 52);

    ofAddListener(backBtn.clickedInside, this, &VideoEditor::gotoPreviousPage);
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
    backBtn.draw();
    gui.draw(); // Draw the GUI on top of the image
}

void VideoEditor::mouseReleased(int x, int y, int button) {
    backBtn.mouseReleased(x, y, button);
}

void VideoEditor::gotoPreviousPage() {
    int PAGE = MAIN_PAGE;
    ofNotifyEvent(redirectEvent, PAGE, this);
}


void VideoEditor::exit() {
    video->exit();
    ofRemoveListener(backBtn.clickedInside, this, &VideoEditor::gotoPreviousPage);
}