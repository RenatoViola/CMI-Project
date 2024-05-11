#pragma once
#include "ofMain.h"

class Media
{
public:
    virtual ~Media() {};

    // Pure virtual methods
    virtual void load(const std::string& filePath) = 0;
    virtual void draw(float x, float y, float w, float h) = 0;
    virtual void update() = 0;
    virtual void drawInFullscreen() = 0;
};

