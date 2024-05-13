#pragma once
#include "Media.h"

class ImageMedia : public Media
{
private:
    ofImage image;

public:
    void load(const std::string& filePath) override {
        image.load(filePath);
        font.load("Courier New Bold.ttf", 9);
        asciiCharacters = string("  ..,,,'''``--_:;^^**""=+<>iv%&xclrs)/){}I?!][1taeo7zjLunT#@JCwfy325Fp6mqSghVd4EgXPGZbYkOA8U$KHDBWNMR0Q");
    }

    void draw(float x, float y, float w, float h) override {
        image.draw(x, y, w, h);
    }

    void update() override {
        // No-op since ofImage doesn't require updating.
    }

    void drawInFullscreen(float sWidth, float sHeight) override {
        float iWidth = image.getWidth(), iHeight = image.getHeight();

        float scale = 1.0f;

        if (iWidth > sWidth || iHeight > sHeight)
            scale = std::min(sWidth / iWidth, sHeight / iHeight);

        float displayWidth = iWidth * scale, displayHeight = iHeight * scale;

        float xPos = (sWidth - displayWidth) / 2.0f, yPos = (sHeight - displayHeight) / 2.0f;

        ofSetColor(ofColor::white);
        image.draw(xPos, yPos, displayWidth, displayHeight);

    float getWidth() {
        return image.getWidth();
    }

    float getHeight() {
        return image.getHeight();
    }

    void exit() override {
        image.clear();
    }

    void drawInAscii(float sWidth, float sHeight)
    {
    }

    ofImage getContent() {
        return image;
    }
};

