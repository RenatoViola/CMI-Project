#pragma once

#include "ofMain.h"
#include "DataTypes/VideoMedia.h"
#include "UIElements/FilterPanel.h"
#include <UIElements/Button.h>
#include <ofxOpenCv.h>

class VideoEditor {
public:
    void setup(VideoMedia* img);
    void update();
    void draw();
    void exit();
    void mouseReleased(int x, int y, int button);
    void gotoHomePage();
    void gotoVersionPage();

    ofEvent<int> redirectEvent;
private:
    VideoMedia* video;
    ofPixels frame;
    FilterPanel gui;
    Button homeBtn, versionBtn;
};