#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include <filter_names.h>
#include "Carrousel.h"

class FilterPanel
{
public:
	void setup();
	void draw();
	
	void colapseButton();

	bool panelVisibility;

	ofxColorSlider color;

	ofxToggle invertColorFilter;
	ofxToggle asciiFilter;
	ofxToggle edgeFilter;

	ofxPanel gui;
};

