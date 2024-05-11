#pragma once
#include "Carrousel.h"
#include "Metadata.h"
#include "FilterPanel.h"

class ImageCarrousel : public Carrousel
{
public:
	virtual void setup(const std::string& path) override;
//	virtual void exit();
	virtual void draw();
	virtual void displayCurrent();

//	virtual void mousePressed(ofMouseEventArgs& args);
	//virtual void mouseReleased(ofMouseEventArgs& args);

	virtual void next();
	virtual void previous();

	vector<ofImage> images;
	FilterPanel panel;

};

