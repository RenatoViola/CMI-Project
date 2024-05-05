#pragma once

#include "ofMain.h"


class Metadata
{
	public:
		static void load(string filename, ofXml XML, bool isImage);
		static void save(string filename, ofXml XML, bool isImage);
		static vector<string> getTags(ofXml XML);

	private:
		static void createFile(string filename, ofXml XML, bool isImage);
};

