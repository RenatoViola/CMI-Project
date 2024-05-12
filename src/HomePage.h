#pragma once

#include "ofMain.h"
#include "Carrousel.h"

class HomePage {
public:
    void setup();
    void exit();
    void update();
    void draw();
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void clickOnImage();
    void clickOnVideo();

    ofEvent<int> clickedOnMedia;

private:
    Carrousel imageCarrousel, videoCarrousel;

};
