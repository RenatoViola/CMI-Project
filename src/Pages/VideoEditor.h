#pragma once

#include "ofMain.h"
#include "DataTypes/VideoMedia.h"
#include "UIElements/FilterPanel.h"
#include "UIElements/Button.h"
#include "ofxOpenCv.h"
#include "MediaEditor.h"

class VideoEditor : public MediaEditor {
public:

    void setup(string filePath) {

        MediaEditor::setup(filePath);

        video.load(filePath);
        video.update();

        Media::setFullScreenSizeAndPos(video.getWidth(), video.getHeight(), &displayWidth, &displayHeight, &xPos, &yPos);
        colorImg.allocate(video.getWidth(), video.getHeight());
    }

    void update() {
        video.update();

        if (video.getContent().isFrameNew()) {
            colorImg.setFromPixels(video.getPixels());

            if (gui.invertColorFilter)
            {
                colorImg.invert();
            }
            else if (gui.edgeFilter) {
                MediaEditor::findContours();
            }
            else 
            {
                // other stuff
            }
        }
    }

    void draw() override {

        video.play();
        ofSetColor(ofColor::black);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
        ofSetColor(ofColor::white);

        if (gui.invertColorFilter)
        {
            colorImg.draw(xPos, yPos, displayWidth, displayHeight);
        }
        else if (gui.edgeFilter)
        {
            MediaEditor::drawEdges();
        }
        else {
            video.draw(xPos, yPos, displayWidth, displayHeight);
        }

        homeBtn.draw();
        versionBtn.draw();
        gui.draw();
    }

private:
    VideoMedia video;
};