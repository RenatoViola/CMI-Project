#pragma once
#include "Carrousel.h"
#include "Metadata.h"
#include "FilterPanel.h"

class ImageCarrousel : public Carrousel
{
public:
	virtual void setup(const std::string& path);
//	virtual void exit();
	virtual void draw();
	virtual void displayCurrent();

	virtual void next();
	virtual void previous();

	vector<ofImage> images;
	FilterPanel panel;

};

