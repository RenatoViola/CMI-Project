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
    virtual void drawInAscii(ofColor backgroundColor) = 0;
    //virtual void drawWithInvertedColors(float sWidth, float sHeight) = 0;

    // characters used in the ascii filter
    string asciiCharacters; 
    ofTrueTypeFont font; // font to be used

    virtual float getWidth() = 0;
    virtual float getHeight() = 0;
    virtual ofPixels& getPixels() = 0;

    void drawInFullscreen(ofColor backgroundColor) {
        float width = this->getWidth(), height = this->getHeight();
        float sWidth = ofGetWidth(), sHeight = ofGetHeight();

        ofSetColor(backgroundColor);
        ofDrawRectangle(0, 0, sWidth, sHeight);

        float scale = 1.0f;

        if (width > sWidth || height > sHeight)
            scale = min(sWidth / width, sHeight / height);

        float displayWidth = width * scale, displayHeight = height * scale;

        float xPos = (sWidth - displayWidth) / 2.0f, yPos = (sHeight - displayHeight) / 2.0f;

        ofSetColor(ofColor::white);
        this->draw(xPos, yPos, displayWidth, displayHeight);
    }

    static void drawInFullscreen(ofPixels& pixels, ofColor backgroundColor) {
        // Convert ofPixels to cv::Mat
        cv::Mat mat(pixels.getHeight(), pixels.getWidth(), CV_8UC(pixels.getNumChannels()), pixels.getData(), 0);

        // Convert the pixel format if necessary
        if (mat.channels() == 4) {
            cv::cvtColor(mat, mat, cv::COLOR_RGBA2RGB);
        }
        else if (mat.channels() == 1) {
            cv::cvtColor(mat, mat, cv::COLOR_GRAY2RGB);
        }

        // Convert cv::Mat back to ofPixels
        ofPixels rgbPixels;
        rgbPixels.allocate(mat.cols, mat.rows, OF_PIXELS_RGB);
        memcpy(rgbPixels.getData(), mat.data, mat.total() * mat.elemSize());

        // Create an ofxCvColorImage and set pixels
        ofxCvColorImage colorImg;
        colorImg.allocate(rgbPixels.getWidth(), rgbPixels.getHeight());
        colorImg.setFromPixels(rgbPixels);

        // Draw background
        float sWidth = ofGetWidth(), sHeight = ofGetHeight();
        ofSetColor(backgroundColor);
        ofDrawRectangle(0, 0, sWidth, sHeight);

        // Calculate scale
        float scale = min(sWidth / colorImg.getWidth(), sHeight / colorImg.getHeight());
        float displayWidth = colorImg.getWidth() * scale, displayHeight = colorImg.getHeight() * scale;

        // Calculate position
        float xPos = (sWidth - displayWidth) / 2.0f;
        float yPos = (sHeight - displayHeight) / 2.0f;

        // Draw the image
        ofSetColor(ofColor::white);
    //    colorImg.invert();  // for experiment sake
        colorImg.draw(xPos, yPos, displayWidth, displayHeight);
    }

};

