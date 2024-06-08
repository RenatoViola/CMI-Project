#pragma once

#include "ofMain.h"
#include "DataTypes/VideoMedia.h"
#include "UIElements/FilterPanel.h"
#include "UIElements/Button.h"
#include "ofxOpenCv.h"
#include "MediaEditor.h"

class VideoEditor : public MediaEditor {
public:

    void setup(string filePath, int versionID) {

        MediaEditor::setup(filePath, versionID);

        video.load(filePath);
        video.update();

        Media::setFullScreenSizeAndPos(video.getWidth(), video.getHeight(), &displayWidth, &displayHeight, &xPos, &yPos);
        colorImg.allocate(video.getWidth(), video.getHeight());

        video.play();

        int b_radius = 300;
        float b_width, b_height, b_x, b_y;
        Media::setFullScreenSizeAndPos(b_radius, b_radius, &b_width, &b_height, &b_x, &b_y);
        playButton.setup("icons/resumeIcon.png", b_radius, b_x, b_y);

        ofAddListener(playButton.clickedInside, this, &VideoEditor::toggleVideoReproduction);
    }

    void update() {
        video.update();
        playButton.update();

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
        playButton.timedDraw();
    }

    void mouseReleased(int x, int y, int button) {
        MediaEditor::mouseReleased(x, y, button);
        playButton.mouseReleased(x, y, button);
    }

    void exit() {
        MediaEditor::exit();
        video.stop();
        ofRemoveListener(playButton.clickedInside, this, &VideoEditor::toggleVideoReproduction);
    }

    void toggleVideoReproduction() {
        video.toggleReproduction();
        playButton.loadIcon(video.isPaused() ? "icons/playIcon.png" : "icons/resumeIcon.png");
    }

private:
    VideoMedia video;
    Button playButton;
};