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


    std::string toString() const {
        std::ostringstream oss;

        oss << "Path: " << path << "\n";
        oss << "Tags: ";
        for (const auto& tag : tags) {
            oss << tag << " ";
        }
        oss << "\n";
        oss << "Luminance: " << luminance << "\n";
        oss << "Color (RGB): " << red << ", " << green << ", " << blue << "\n";
        oss << "Number of Faces: " << numFaces << "\n";

        oss << "Edge Distribution: \n";
        for (const auto& edge : edgeDistribution) {
            oss << "  (" << std::get<0>(edge) << ", " << std::get<1>(edge) << ", " << std::get<2>(edge) << ")\n";
        }

        oss << "Texture Characteristics: \n";
        for (const auto& texture : textureCharacteristics) {
            oss << "  (" << std::get<0>(texture) << ", " << std::get<1>(texture) << ", " << std::get<2>(texture) << ")\n";
        }

        return oss.str();
    }
};

#endif

