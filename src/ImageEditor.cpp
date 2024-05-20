#include "ImageEditor.h"

void ImageEditor::setup(ImageMedia* img) {
    // Assign the provided ImageMedia object to the class member
    originalImage = img;
    currentImage = img;

    gui.setup();

    backBtn.setup("backIcon.png", 100, 45, 52);

    ofAddListener(backBtn.clickedInside, this, &ImageEditor::gotoPreviousPage);
}

void ImageEditor::update() {
    // Here you would typically handle real-time adjustments to the image
    // For example, applying brightness or contrast settings
 //   image->update();
    gui.update();
}

void ImageEditor::draw() {

    if (gui.asciiFilter)
    {
        currentImage->drawInAscii(ofColor::black);
    } 
    else if (gui.invertColorFilter)
    {
        ofxCvColorImage i;
        i.setFromPixels(currentImage->getPixels());
        i.invert();
        Media::drawInFullscreen(i.getPixels(), ofColor::black);
    }
    else {
        currentImage->drawInFullscreen(ofColor::black);
    }

//    Media::drawInFullscreen(image->getPixels(), ofColor::black);
    backBtn.draw();
    gui.draw();
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
}