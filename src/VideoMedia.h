#pragma once
#include "Media.h"

class VideoMedia : public Media
{
private:
    bool frameByframe = false;
    ofVideoPlayer video;

public:
    void load(const std::string& filePath) override {
        video.load(filePath);
        video.setLoopState(OF_LOOP_NORMAL);
        video.play();  // Optionally start playing automatically
        font.load("Courier New Bold.ttf", 9);
        asciiCharacters = string("  ..,,,'''``--_:;^^**""=+<>iv%&xclrs)/){}I?!][1taeo7zjLunT#@JCwfy325Fp6mqSghVd4EgXPGZbYkOA8U$KHDBWNMR0Q");
    }

    void draw(float x, float y, float w, float h) override {
        video.draw(x, y, w, h);
    }

    void update() override {
        video.update();
    }

    void drawInFullscreen(float sWidth, float sHeight) override {
        float vWidth = video.getWidth(), vHeight = video.getHeight();

        float scale = 1.0f;

        if (vWidth > sWidth || vHeight > sHeight)
            scale = std::min(sWidth / vWidth, sHeight / vHeight);

        float displayWidth = vWidth * scale, displayHeight = vHeight * scale;

        float xPos = (sWidth - displayWidth) / 2.0f, yPos = (sHeight - displayHeight) / 2.0f;

        ofSetColor(ofColor::white);
        video.draw(xPos, yPos, displayWidth, displayHeight);
    }


    void play() {
        video.play();
    }

    void pause() {
        frameByframe = !frameByframe;
        video.setPaused(frameByframe);
    }

    static vector<ofPixels> extractFrames(ofVideoPlayer& video, int skip) {
        vector<ofPixels> frames;

        video.update();
        video.play();

        ofSleepMillis(500);

        video.setPaused(true);
        video.update();

        int currentFrame = 0;
        int totalFrames = video.getTotalNumFrames();

        while (currentFrame < totalFrames) {
            if (currentFrame % (skip + 1) == 0) {
                while (!video.isFrameNew())
                {
                    video.update();
                }
                frames.push_back(video.getPixels());
            }
            video.nextFrame();
            currentFrame++;
        }
        video.stop();

        return frames;
    }

    void drawInAscii(float sWidht, float sHeight) override
    {

    }


    ofVideoPlayer getContent() {
        return video;
    }
};
