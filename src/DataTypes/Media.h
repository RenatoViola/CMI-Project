#pragma once
#include "ofMain.h"
#include <ofxOpenCv.h>

class Media
{
public:
    ~Media() {};

    // Pure virtual methods
    virtual void load(const string& filePath) = 0;
    virtual void exit() = 0;
    virtual void draw(float x, float y, float w, float h) = 0;
    virtual void update() = 0;

    // characters used in the ascii filter
    string asciiCharacters;
    ofTrueTypeFont font; // font to be used

    virtual float getWidth() = 0;
    virtual float getHeight() = 0;
    virtual ofPixels& getPixels() = 0;

    static void setFullScreenSizeAndPos(float width, float height, float* displayWidth, float* displayHeight, float* xPos, float* yPos)
    {
        float sWidth = ofGetWidth(), sHeight = ofGetHeight();

        float scale = 1.0f;

        if (width > sWidth || height > sHeight)
            scale = min(sWidth / width, sHeight / height);

        *displayWidth = width * scale;
        *displayHeight = height * scale;

        *xPos = (sWidth - *displayWidth) / 2.0f;
        *yPos = (sHeight - *displayHeight) / 2.0f;
    }
};

