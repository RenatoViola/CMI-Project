#pragma once

#include "ofMain.h"
#include <ofxOpenCv.h>
#include "DataTypes/ImageMedia.h"
#include "UIElements/FilterPanel.h"
#include <UIElements/Button.h>
#include "MediaEditor.h"

class ImageEditor : public MediaEditor {
public:

    void setup(ImageMedia* media) {
        img = media;
        MediaEditor::setup(img);
        
        gui.invertColorFilter.addListener(this, &ImageEditor::invertImage);
        gui.edgeFilter.addListener(this, &ImageEditor::edgeImage);
    }

    void draw() override {

        ofSetColor(ofColor::black);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
        ofSetColor(ofColor::white);

        if (gui.asciiFilter)
        {
            img->drawInAscii();
        }
        else if (gui.invertColorFilter)
        {
            colorImg.draw(xPos, yPos, displayWidth, displayHeight);
        }
        else if (gui.edgeFilter)
        {
            MediaEditor::drawEdges();
        }
        else {
            img->draw(xPos, yPos, displayWidth, displayHeight);
        }

        homeBtn.draw();
        versionBtn.draw();
        gui.draw();
    }

    void exit() {
        gui.invertColorFilter.removeListener(this, &ImageEditor::invertImage);
        gui.edgeFilter.removeListener(this, &ImageEditor::edgeImage);
        MediaEditor::exit();
    }

    void invertImage(bool& toggleValue) {
        if (toggleValue)
        {
            colorImg.invert();
        }
        else
        {
            colorImg.setFromPixels(img->getPixels());
        }
    }

    void edgeImage(bool& toggleValue) {
        if (toggleValue)
        {
            MediaEditor::findContours();
        }
        else
        {
            colorImg.setFromPixels(img->getPixels());
        }
    }

private:
    ImageMedia* img;
};
