#pragma once

#include "ofMain.h"


class FilesMetadata
{
	public:
		void setup();
		void load(string filename);
		void save(string filename);
		void createFile(string filename, vector<string> tags);
		vector<string> getFileTags();
		void addTag(string tag);


		ofXml xml;
		std::string currentFileName;
		std::string xmlText;
};

