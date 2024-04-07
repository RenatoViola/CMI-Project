#pragma once
#include "Carrousel.h"

class VideoCarrousel : public Carrousel
{
public:
	virtual void setup(const std::string& path);
	virtual void update();
	virtual void draw(int numFilesToShow, int width, int height, int gridSpacing);

	virtual void next();
	virtual void previous();

	void pause();
	void nextFrame();
	void previousFrame();
	void firstFrame();

	bool frameByframe;
	vector<ofVideoPlayer> videos;
};

