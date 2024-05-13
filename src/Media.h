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
    //virtual void drawInFullscreen() = 0;
    virtual void drawInFullscreen(float sWidth, float sHeight) = 0;

    // filters to be applied to image
    virtual void drawInAscii(float sWidth, float sHeight) = 0;
    //virtual void drawWithInvertedColors(float sWidth, float sHeight) = 0;

    // characters used in the ascii filter
    string asciiCharacters; 
    ofTrueTypeFont font; // font to be used
};

