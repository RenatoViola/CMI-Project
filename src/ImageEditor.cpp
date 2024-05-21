#include "ImageEditor.h"

void ImageEditor::setup(ImageMedia* img) {
    // Assign the provided ImageMedia object to the class member
    this->img = img;

    colorImg.allocate(img->getWidth(), img->getHeight());
    colorImg.setFromPixels(img->getPixels());

    gui.setup();

    homeBtn.setup("homeIcon.png", 100, 50, 50);
    versionBtn.setup("versionIcon.png", 100, 50, 200);

    ofAddListener(homeBtn.clickedInside, this, &ImageEditor::gotoPreviousPage);
    ofAddListener(versionBtn.clickedInside, this, &ImageEditor::gotoVersionPage);

    gui.invertColorFilter.addListener(this, &ImageEditor::invertImage);
}

void ImageEditor::update() {
 //   img->update();
    gui.update();
 //   ofLogNotice() << "FPS: " << ofGetFrameRate();
}

void ImageEditor::draw() {

    if (gui.asciiFilter)
    {
        img->drawInAscii(ofColor::black);
    } 
    else if (gui.invertColorFilter)
    {
        Media::drawInFullscreen(colorImg, ofColor::black);
    }
    else {
        img->drawInFullscreen(ofColor::black);
    }

    homeBtn.draw();
    versionBtn.draw();

    gui.draw();
}

void ImageEditor::invertImage(bool& toggleValue) {
    colorImg.invert();
}

void ImageEditor::mouseReleased(int x, int y, int button) {
    homeBtn.mouseReleased(x, y, button);
    versionBtn.mouseReleased(x, y, button);
}

void ImageEditor::gotoPreviousPage() {
    int PAGE = MAIN_PAGE;
    ofNotifyEvent(redirectEvent, PAGE, this);
}

void ImageEditor::gotoVersionPage() {
    int PAGE = CONTROL_VERSION_PAGE;
    ofNotifyEvent(redirectEvent, PAGE, this);
}

void ImageEditor::exit() {
    //    image->exit();
    ofRemoveListener(homeBtn.clickedInside, this, &ImageEditor::gotoPreviousPage);
    ofRemoveListener(versionBtn.clickedInside, this, &ImageEditor::gotoVersionPage);
    gui.invertColorFilter.removeListener(this, &ImageEditor::invertImage);
}