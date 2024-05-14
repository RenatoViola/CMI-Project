#pragma once
#include "Media.h"

class ImageMedia : public Media
{
private:
    ofImage image;

public:
    void load(const std::string& filePath) {
        image.load(filePath);
    }

    void draw(float x, float y, float w, float h) {
        image.draw(x, y, w, h);
    }

    void update() {
        // No-op since ofImage doesn't require updating.
    }

    float getWidth() {
        return image.getWidth();
    }

    float getHeight() {
        return image.getHeight();
    }

    void exit() {
    //    image.clear();
    }

    ofImage getContent() {
        return image;
    }

    ofPixels& getPixels() {
        return image.getPixels();
    }
};