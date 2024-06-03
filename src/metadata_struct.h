#pragma once
#include "ofMain.h"

#ifndef METADATA_STRUCT_H
#define METADATA_STRUCT_H

struct FileMetadata {
	string path;
	set<string> tags;
	double luminance;
	int red, green, blue;
	int numFaces;
	array<tuple<int, double, double>, 5> edgeDistribution;
	array<tuple<int, double, double>, 24> textureCharacteristics;
};

#endif

