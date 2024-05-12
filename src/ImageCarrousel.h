#pragma once
#include "Carrousel.h"
#include "Metadata.h"
#include "FilterPanel.h"

class ImageCarrousel : public Carrousel
{
public:
	void setup(const std::string& path);
//	virtual void exit();
	void draw();
	void displayCurrent();

	void next();
	void previous();

	vector<ofImage> images;
	FilterPanel panel;

};

