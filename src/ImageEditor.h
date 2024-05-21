#pragma once

#include "ofMain.h"
#include "ImageMedia.h"
#include "FilterPanel.h"
#include <ofxOpenCv.h>
#include <Button.h>

class ImageEditor {
public:
    void setup(ImageMedia* img);
    void update();
    void draw();
    void exit();
    void mouseReleased(int x, int y, int button);
    void gotoPreviousPage();
    void gotoVersionPage();
    void invertImage(bool& toggleValue);

    ofEvent<int> redirectEvent;

private:
    ImageMedia* img;
    ofxCvColorImage colorImg;
    FilterPanel gui;
    Button homeBtn, versionBtn;
};
