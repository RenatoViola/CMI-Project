#pragma once
#include "Media.h"

class ImageMedia : public Media
{
private:
    ofImage image;

public:
    void load(const std::string& filePath) {
        image.load(filePath);
        font.load("Courier New Bold.ttf", 9);
        asciiCharacters = string("  ..,,,'''``--_:;^^**""=+<>iv%&xclrs)/){}I?!][1taeo7zjLunT#@JCwfy325Fp6mqSghVd4EgXPGZbYkOA8U$KHDBWNMR0Q");
    }

    void load(ofImage img)
    {
        image.setFromPixels(img.getPixels());
        font.load("Courier New Bold.ttf", 9);
        asciiCharacters = string("  ..,,,'''``--_:;^^**""=+<>iv%&xclrs)/){}I?!][1taeo7zjLunT#@JCwfy325Fp6mqSghVd4EgXPGZbYkOA8U$KHDBWNMR0Q");
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

    void drawInAscii(ofColor backgroundColor)
    {
        float width = image.getWidth(), height = image.getHeight();
        float sWidth = ofGetWidth(), sHeight = ofGetHeight();

        ofSetColor(backgroundColor);
        ofDrawRectangle(0, 0, sWidth, sHeight);

        float scale = 1.0f;

        if (width > sWidth || height > sHeight)
            scale = min(sWidth / width, sHeight / height);

        float displayWidth = width * scale, displayHeight = height * scale;

        float xPos = (sWidth - displayWidth) / 2.0f, yPos = (sHeight - displayHeight) / 2.0f;
        
        ofPixelsRef pixelsRef = image.getPixelsRef();
        ofSetColor(ofColor::white);

        for (int i = 0; i < displayWidth; i += 7) {
            for (int j = 0; j < displayHeight; j += 9) {
                
                float lightness = pixelsRef.getColor(i, j).getLightness();

                // calculate the index of the character from our asciiCharacters array
                int character = powf(ofMap(lightness, 0, 255, 0, 1), 2.5) * asciiCharacters.size();


                // draw the character at the correct location
                font.drawString(ofToString(asciiCharacters[character]), i + xPos,  j + yPos);
            }
        }
    }

    ofImage getContent() {
        return image;
    }

    ofPixels& getPixels() {
        return image.getPixels();
    }
};