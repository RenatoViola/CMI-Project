#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include <filter_names.h>
#include "Carrousel.h"

class FilterPanel
{
public:
	void setup(const string& filePath);
	void draw();
	void exit();
	
	void collapseButton();

	void saveButtonPressed();
	void loadVersionInfo(int versionID);

	bool panelVisibility, isImageFile;

	ofxToggle invertColorFilter, asciiFilter, edgeFilter, blurFilter, blurGaussianFilter, dilateFilter, erodeFilter;
	ofxLabel filtersLabel, changesLabel, spaceLabel;

	ofxPanel gui;

	ofxButton saveBtn;

	string filePath;
};