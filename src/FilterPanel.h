#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Carrousel.h"

class FilterPanel
{
public:
	void setup();
	void update();
	void draw();
	
	void colapseButton();


	bool panelVisibility;

	ofxColorSlider color;

	ofxToggle invertColorFilter;
	ofxToggle asciiFilter;
	
	ofxButton twoCircles;
	ofxButton ringButton;
	ofxLabel screenSize;

	ofxPanel gui;
};

