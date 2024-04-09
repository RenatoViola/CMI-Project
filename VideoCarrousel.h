#pragma once
#include "Carrousel.h"

class VideoCarrousel : public Carrousel
{
public:
	virtual void setup(const std::string& path);
	virtual void draw(int numFilesToShow, int width, int height, int gridSpacing);
	virtual void displayCurrent();

	virtual void next();
	virtual void previous();

	void update();

	void pause();
	void nextFrame();
	void previousFrame();
	void firstFrame();

	bool frameByframe;
	vector<ofVideoPlayer> videos;
};
