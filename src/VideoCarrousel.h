#pragma once
#include "Carrousel.h"
#include "Metadata.h"

class VideoCarrousel : public Carrousel
{
public:
	virtual void setup(const std::string& path) override;
	virtual void draw();
	virtual void displayCurrent();

	virtual void next();
	virtual void previous();

	void update();

	void pause();
	void nextFrame();
	void previousFrame();
	void firstFrame();

	bool frameByframe = false;

	vector<ofVideoPlayer> videos;
};

