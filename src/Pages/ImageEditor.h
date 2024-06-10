#pragma once
#include "DataTypes/ImageMedia.h"
#include <MediaEditor.h>

class ImageEditor : public MediaEditor {
public:

    void setup(const string& filePath, int versionID)
    {
        MediaEditor::setup(filePath, versionID);

        img.load(filePath);

        Media::setFullScreenSizeAndPos(img.getWidth(), img.getHeight(), &displayWidth, &displayHeight, &xPos, &yPos);
        colorImg.allocate(img.getWidth(), img.getHeight());
        colorImg.setFromPixels(img.getPixels());

        setupVersion();
        
        gui.edgeFilter.addListener(this, &ImageEditor::edge);
        gui.invertColorFilter.addListener(this, &ImageEditor::invert);
        gui.blurFilter.addListener(this, &ImageEditor::blur);
        gui.blurGaussianFilter.addListener(this, &ImageEditor::gaussianBlur);
        gui.dilateFilter.addListener(this, &ImageEditor::dilate);
        gui.erodeFilter.addListener(this, &ImageEditor::erode);
    }


    void draw() 
    {
        ofSetColor(ofColor::black);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
        ofSetColor(ofColor::white);

        if (gui.asciiFilter)
        {
            img.drawInAscii();
        }
        else if (gui.edgeFilter)
        {
            MediaEditor::drawEdges();
        }
        else if (
                gui.invertColorFilter 
                || gui.blurFilter 
                || gui.blurGaussianFilter 
                || gui.dilateFilter 
                || gui.erodeFilter
            )
        {
            colorImg.draw(xPos, yPos, displayWidth, displayHeight);
        }
        else {
            img.draw(xPos, yPos, displayWidth, displayHeight);
        }

        homeBtn.draw();
        versionBtn.draw();
        gui.draw();
    }


    void exit() 
    {
        gui.invertColorFilter.removeListener(this, &ImageEditor::invert);
        gui.edgeFilter.removeListener(this, &ImageEditor::edge);
        gui.blurFilter.removeListener(this, &ImageEditor::blur);
        gui.blurGaussianFilter.removeListener(this, &ImageEditor::gaussianBlur);
        gui.dilateFilter.removeListener(this, &ImageEditor::dilate);
        gui.erodeFilter.removeListener(this, &ImageEditor::erode);
        MediaEditor::exit();
    }


private:
    void setupVersion() {
        bool b_value = true;

        if (gui.edgeFilter)
        {
            edge(b_value);
        }
        else if (gui.invertColorFilter)
        {
            invert(b_value);
        }
        else if (gui.blurFilter)
        {
            blur(b_value);
        }
        else if (gui.blurGaussianFilter)
        {
            gaussianBlur(b_value);
        }
        else if (gui.dilateFilter)
        {
            dilate(b_value);
        }
        else if (gui.erodeFilter)
        {
            erode(b_value);
        }
        else
        {
            return;
        }
    }


    void edge(bool& toggleValue) {
        toggleValue ? MediaEditor::findContours() : colorImg.setFromPixels(img.getPixels());
    }


    void invert(bool& toggleValue) {
        toggleValue ? colorImg.invert() : colorImg.setFromPixels(img.getPixels());
    }


    void blur(bool& toggleValue) {
        toggleValue ? colorImg.blur() : colorImg.setFromPixels(img.getPixels());
    }


    void gaussianBlur(bool& toggleValue) {
        toggleValue ? colorImg.blurGaussian() : colorImg.setFromPixels(img.getPixels());
    }


    void dilate(bool& toggleValue) {
        toggleValue ? colorImg.dilate() : colorImg.setFromPixels(img.getPixels());
    }


    void erode(bool& toggleValue) {
        toggleValue ? colorImg.erode() : colorImg.setFromPixels(img.getPixels());
    }


    ImageMedia img;
};
