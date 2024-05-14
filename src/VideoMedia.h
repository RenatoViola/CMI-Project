#pragma once
#include "Media.h"

class VideoMedia : public Media
{
private:
    bool frameByframe = false;
    ofVideoPlayer video;

public:
    void load(const std::string& filePath) {
        video.load(filePath);
    }

    void draw(float x, float y, float w, float h) {
        video.draw(x, y, w, h);
    }

    void update() {
        video.update();
    }

    void exit() {
    //    video.close();
        video.stop();
    }

    float getWidth() {
        return video.getWidth();
    }

    float getHeight() {
        return video.getHeight();
    }

    void play() {
        video.setLoopState(OF_LOOP_NORMAL);
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

    ofVideoPlayer getContent() {
        return video;
    }

    ofPixels& getPixels() {
        return video.getPixels();
    }
};
