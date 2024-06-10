#pragma once

#include "ofMain.h"
#include "UIElements/Carrousel.h"
#include <UIElements/Button.h>

class HomePage {
public:
    void setup();
    void exit();
    void update();
    void draw();
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    string getCurrentFilePath();

    ofEvent<int> redirectEvent;
   
private:
    void gotoImagePage();
    void gotoVideoPage();
    void gotoCameraPage();
    void gotoSavescreenPage();

    string selectedFilePath;
    Carrousel imageCarrousel, videoCarrousel;
    Button cameraBtn, lockBtn;
};
