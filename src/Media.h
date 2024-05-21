#pragma once
#include "ofMain.h"
#include <ofxOpenCv.h>

class Media
{
public:
    virtual ~Media() {};

    // Pure virtual methods
    virtual void load(const std::string& filePath) = 0;
    virtual void exit() = 0;
    virtual void draw(float x, float y, float w, float h) = 0;
    virtual void update() = 0;

    // filters to be applied to image
 //   virtual void drawInAscii(ofColor backgroundColor) = 0;
    //virtual void drawWithInvertedColors(float sWidth, float sHeight) = 0;

    // characters used in the ascii filter
    string asciiCharacters;
    ofTrueTypeFont font; // font to be used

    virtual float getWidth() = 0;
    virtual float getHeight() = 0;
    virtual ofPixels& getPixels() = 0;

    static void setFullScreenSizeAndPos(ofxCvColorImage img, float* displayWidth, float* displayHeight, float* xPos, float* yPos)
    {
        float sWidth = ofGetWidth(), sHeight = ofGetHeight();
        float width = img.getWidth(), height = img.getHeight();

        float scale = 1.0f;

        if (width > sWidth || height > sHeight)
            scale = min(sWidth / width, sHeight / height);

        *displayWidth = width * scale;
        *displayHeight = height * scale;

        *xPos = (sWidth - *displayWidth) / 2.0f;
        *yPos = (sHeight - *displayHeight) / 2.0f;
    }

    void setFullScreenSizeAndPos(float* displayWidth, float* displayHeight, float* xPos, float* yPos)
    {
        float sWidth = ofGetWidth(), sHeight = ofGetHeight();
        float width = this->getWidth(), height = this->getHeight();

        float scale = 1.0f;

        if (width > sWidth || height > sHeight)
            scale = min(sWidth / width, sHeight / height);

        *displayWidth = width * scale;
        *displayHeight = height * scale;

        *xPos = (sWidth - *displayWidth) / 2.0f;
        *yPos = (sHeight - *displayHeight) / 2.0f;
    }

    void drawInFullscreen(ofColor backgroundColor)
    {
        float sWidth = ofGetWidth(), sHeight = ofGetHeight();
        float displayWidth, displayHeight, xPos, yPos;
        this->setFullScreenSizeAndPos(&displayWidth, &displayHeight, &xPos, &yPos);

        ofSetColor(backgroundColor);
        ofDrawRectangle(0, 0, sWidth, sHeight);
        ofSetColor(ofColor::white);
        this->draw(xPos, yPos, displayWidth, displayHeight);
    }

    static void drawInFullscreen(ofxCvColorImage img, ofColor backgroundColor)
    {
        float sWidth = ofGetWidth(), sHeight = ofGetHeight();
        float displayWidth, displayHeight, xPos, yPos;
        setFullScreenSizeAndPos(img, &displayWidth, &displayHeight, &xPos, &yPos);

        ofSetColor(backgroundColor);
        ofDrawRectangle(0, 0, sWidth, sHeight);
        ofSetColor(ofColor::white);
        img.draw(xPos, yPos, displayWidth, displayHeight);
    }

};

