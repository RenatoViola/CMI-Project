#pragma once

#include "ofMain.h"
#include <ofxOpenCv.h>
#include "DataTypes/ImageMedia.h"
#include "UIElements/FilterPanel.h"
#include <UIElements/Button.h>

class ImageEditor {
public:
    void setup(ImageMedia* img);
    void update();
    void draw();
    void exit();
    void mouseReleased(int x, int y, int button);
    void gotoPreviousPage();
    void invertImage(bool& toggleValue);

    ofEvent<int> redirectEvent;

private:
    ImageMedia* img;
    ofxCvColorImage colorImg;
    FilterPanel gui;
    Button backBtn;
};
