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

    void gotoImagePage();
    void gotoVideoPage();
    void gotoCameraPage();
    void gotoSavescreenPage();
    string getCurrentFilePath();

    ofEvent<int> redirectEvent;
   
private:
    Carrousel imageCarrousel, videoCarrousel;
    string selectedFilePath;
    Button cameraBtn, lockBtn;
};
