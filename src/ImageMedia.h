#pragma once
#include "Media.h"

class ImageMedia : public Media
{
private:
    ofImage image;

public:
    void load(const std::string& filePath) override {
        image.load(filePath);
    }

    void draw(float x, float y, float w, float h) override {
        image.draw(x, y, w, h);
    }

    void update() override {
        // No-op since ofImage doesn't require updating.
    }

    void exit() override {
        image.clear();
    }

    void drawInFullscreen() override {
        float iWidth = image.getWidth(), iHeight = image.getHeight();

        float sWidth = ofGetWidth(), sHeight = ofGetHeight();

        ofSetColor(ofColor::black);
        ofDrawRectangle(0, 0, sWidth, sHeight);

        float scale = 1.0f;

        if (iWidth > sWidth || iHeight > sHeight)
            scale = std::min(sWidth / iWidth, sHeight / iHeight);

        float displayWidth = iWidth * scale, displayHeight = iHeight * scale;

        float xPos = (sWidth - displayWidth) / 2.0f, yPos = (sHeight - displayHeight) / 2.0f;

        ofSetColor(ofColor::white);
        image.draw(xPos, yPos, displayWidth, displayHeight);
    }

    ofImage getContent() {
        return image;
    }
};

