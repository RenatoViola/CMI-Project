#pragma once
#include "ofMain.h"
#include "FilterPanel.h"
#include "Media.h"
#include "Metadata.h"
#include "ofApp.h"

class MediaEditor
{
public:

	void setup();
	void load(Media * m);
	void update();
	void draw();
	void exit();

	FilterPanel panel;
	Media* media;
	Metadata metadata;

};

