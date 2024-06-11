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
	void loadVersionInfo(int versionID);
	void exit();

	ofxToggle invertColorFilter, asciiFilter, edgeFilter, blurFilter, blurGaussianFilter, dilateFilter, erodeFilter;

private:
	void saveButtonPressed();
	
	bool panelVisibility, isImageFile;
	ofxLabel filtersLabel, changesLabel, spaceLabel;
	ofxPanel gui;
	ofxButton saveBtn;
	string filePath;
};