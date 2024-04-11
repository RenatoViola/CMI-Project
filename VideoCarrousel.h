#pragma once
#include "Carrousel.h"

class VideoCarrousel : public Carrousel
{
public:
	virtual void setup(const std::string& path);
	virtual void drawAsList(int numFilesToShow, int width, int height, int gridSpacing);
	virtual void drawAsRevolver(int radius, int width, int height);
	virtual void displayCurrent();

	virtual void next();
	virtual void previous();

	void update();

	void pause();
	void nextFrame();
	void previousFrame();
	void firstFrame();

	bool frameByframe;

	float currentWidth;
	float currentHeight;

	vector<ofVideoPlayer> videos;
};

