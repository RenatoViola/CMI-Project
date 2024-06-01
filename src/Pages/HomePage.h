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
    Media* getSelectedMedia();

    ofEvent<int> redirectEvent;
   
private:
    Carrousel imageCarrousel, videoCarrousel;
    Media* selectedMedia;
    Button cameraBtn, lockBtn;
};
