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
    Media* getSelectedMedia();

    ofEvent<int> redirectEvent;
   
private:
    Carrousel imageCarrousel, videoCarrousel;
    Media* selectedMedia;
};
