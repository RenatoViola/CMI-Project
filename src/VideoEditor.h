#pragma once

#include "ofMain.h"
#include "VideoMedia.h"
#include "FilterPanel.h"
#include <Button.h>
#include <ofxOpenCv.h>

class VideoEditor {
public:
    void setup(VideoMedia* img);
    void update();
    void draw();
    void exit();
    void mouseReleased(int x, int y, int button);
    void gotoPreviousPage();

    ofEvent<int> redirectEvent;
private:
    VideoMedia* video;
    ofPixels frame;
    FilterPanel gui;
    Button backBtn;
};
