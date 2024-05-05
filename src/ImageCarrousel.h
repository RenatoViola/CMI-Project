#pragma once
#include "Carrousel.h"
#include "Metadata.h"

class ImageCarrousel : public Carrousel
{
public:
	virtual void setup(const std::string& path);
	virtual void draw(int numFilesToShow, int width, int height, int gridSpacing);
	virtual void displayCurrent();

	virtual void next();
	virtual void previous();

	vector<ofImage> images;
	ofXml xml;
};

