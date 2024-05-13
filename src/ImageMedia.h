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

    float getWidth() {
        return image.getWidth();
    }

    float getHeight() {
        return image.getHeight();
    }

    void exit() override {
        image.clear();
    }

    ofImage getContent() {
        return image;
    }
};

