#pragma once
#include "ofMain.h"

class Media
{
public:
    virtual ~Media() {};

    // Pure virtual methods
    virtual void load(const std::string& filePath) = 0;
    virtual void exit() = 0;
    virtual void draw(float x, float y, float w, float h) = 0;
    virtual void update() = 0;
    //virtual void drawInFullscreen() = 0;
    virtual void drawInFullscreen(float sWidth, float sHeight) = 0;

    // filters to be applied to image
    virtual void drawInAscii(float sWidth, float sHeight) = 0;
    //virtual void drawWithInvertedColors(float sWidth, float sHeight) = 0;

    // characters used in the ascii filter
    string asciiCharacters; 
    ofTrueTypeFont font; // font to be used

    virtual float getWidth() = 0;
    virtual float getHeight() = 0;

    void drawInFullscreen(ofColor backgroundColor) {
        float width = this->getWidth(), height = this->getHeight();
        float sWidth = ofGetWidth(), sHeight = ofGetHeight();

        ofSetColor(backgroundColor);
        ofDrawRectangle(0, 0, sWidth, sHeight);

        float scale = 1.0f;

        if (width > sWidth || height > sHeight)
            scale = std::min(sWidth / width, sHeight / height);

        float displayWidth = width * scale, displayHeight = height * scale;

        float xPos = (sWidth - displayWidth) / 2.0f, yPos = (sHeight - displayHeight) / 2.0f;

        ofSetColor(ofColor::white);
        this->draw(xPos, yPos, displayWidth, displayHeight);
    }
};

