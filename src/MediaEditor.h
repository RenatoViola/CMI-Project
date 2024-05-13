#pragma once
#include "ofMain.h"
#include "FilterPanel.h"
#include "Media.h"
#include "Metadata.h"

class MediaEditor
{
public:

	void setup();
	void load(Media * m);
	void update();
	void draw();

	FilterPanel panel;
	Media* media;
	Metadata metadata;
};

