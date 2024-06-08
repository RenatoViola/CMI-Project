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
    
    string getFilePath() {
        return filePath;
    }

    string filePath;

    virtual float getWidth() = 0;
    virtual float getHeight() = 0;
    virtual ofPixels& getPixels() = 0;
    virtual ofPixels& getThumbnail() = 0;

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

    static bool isImage(const string& filePath) {
        if (filePath.size() >= 4) {
            if (filePath.substr(filePath.size() - 4) == ".jpg") {
                return true;
            }
            else return false;
        }
    }

    static string getFileName(const string& filePath) {
        size_t pos = filePath.find_last_of("/");
        if (pos == string::npos) {
            return filePath; // No slash found, return the original path
        }
        return filePath.substr(pos + 1);
    }
};

