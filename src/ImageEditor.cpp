#include "ImageEditor.h"

void ImageEditor::setup(ImageMedia* img) {
    // Assign the provided ImageMedia object to the class member
    this->img = img;

    colorImg.allocate(img->getWidth(), img->getHeight());
    colorImg.setFromPixels(img->getPixels());

    gui.setup();

    backBtn.setup("backIcon.png", 100, 50, 50);

    ofAddListener(backBtn.clickedInside, this, &ImageEditor::gotoPreviousPage);
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

    backBtn.draw();
    gui.draw();
}

void ImageEditor::invertImage(bool& toggleValue) {
    colorImg.invert();
}

void ImageEditor::mouseReleased(int x, int y, int button) {
    backBtn.mouseReleased(x, y, button);
}

void ImageEditor::gotoPreviousPage() {
    int PAGE = MAIN_PAGE;
    ofNotifyEvent(redirectEvent, PAGE, this);
}

void ImageEditor::exit() {
    //    image->exit();
    ofRemoveListener(backBtn.clickedInside, this, &ImageEditor::gotoPreviousPage);
    gui.invertColorFilter.removeListener(this, &ImageEditor::invertImage);
}