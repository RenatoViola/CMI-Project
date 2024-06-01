#pragma once
#include "Media.h"

class ImageMedia : public Media
{
private:
    ofImage image;
    vector<string> asciiArt;
    ofFbo asciiFbo;

    void generateAsciiArt() 
    {
        float sWidth = ofGetWidth(), sHeight = ofGetHeight();
        float displayWidth, displayHeight, xPos, yPos;
        setFullScreenSizeAndPos(&displayWidth, &displayHeight, &xPos, &yPos);

        ofImage resizedImg;
        resizedImg.allocate(displayWidth, displayHeight, OF_IMAGE_GRAYSCALE);
        resizedImg.setFromPixels(image.getPixels());
        resizedImg.resize(displayWidth, displayHeight);

        asciiArt.clear();

        for (int j = 0; j < displayHeight; j += 9) {
            string line;
            for (int i = 0; i < displayWidth; i += 7) {
                float lightness = resizedImg.getPixels().getColor(i, j).getLightness();
                int characterIndex = powf(ofMap(lightness, 0, 255, 0, 1), 2.5) * (asciiCharacters.size() - 1);
                characterIndex = ofClamp(characterIndex, 0, asciiCharacters.size() - 1);
                line += asciiCharacters[characterIndex];
            }
            asciiArt.push_back(line);
        }
        resizedImg.clear();

        asciiFbo.allocate(sWidth, sHeight, GL_RGBA);
        asciiFbo.begin();
        ofClear(0, 0, 0, 0);
        ofSetColor(ofColor::white);

        for (int i = 0; i < asciiArt.size(); ++i) {
            font.drawString(asciiArt[i], xPos, yPos + i * 9);
        }
        asciiFbo.end();
    }

public:
    void load(const std::string& filePath) 
    {
        image.load(filePath);
        font.load("Courier New Bold.ttf", 9);
        asciiCharacters = string("  ..,,,'''``--_:;^^**""=+<>iv%&xclrs)/){}I?!][1taeo7zjLunT#@JCwfy325Fp6mqSghVd4EgXPGZbYkOA8U$KHDBWNMR0Q");
        generateAsciiArt();
    }

    void load(ofImage img)
    {
        image.setFromPixels(img.getPixels());
        font.load("Courier New Bold.ttf", 9);
        asciiCharacters = string("  ..,,,'''``--_:;^^**""=+<>iv%&xclrs)/){}I?!][1taeo7zjLunT#@JCwfy325Fp6mqSghVd4EgXPGZbYkOA8U$KHDBWNMR0Q");
        generateAsciiArt();
    }

    void update() 
    {
        // No-op since ofImage doesn't require updating.
    }

    float getWidth() 
    {
        return image.getWidth();
    }

    float getHeight()
    {
        return image.getHeight();
    }

    void exit() 
    {
    //    image.clear();
    }

    void draw(float x, float y, float w, float h)
    {
        image.draw(x, y, w, h);
    }

    void drawInAscii(ofColor backgroundColor)
    {
        ofSetColor(backgroundColor);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

        ofSetColor(ofColor::white);
        asciiFbo.draw(0, 0);
    }
    
    ofImage getContent() 
    {
        return image;
    }

    ofPixels& getPixels() 
    {
        return image.getPixels();
    }
};