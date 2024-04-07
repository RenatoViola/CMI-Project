#pragma once
#include "Carrousel.h"

class ImageCarrousel : public Carrousel
{
public:
	virtual void setup(const std::string& path);
	virtual void update();
	virtual void draw(int numFilesToShow, int width, int height, int gridSpacing);

	virtual void next();
	virtual void previous();

	vector<ofImage> images;
};

