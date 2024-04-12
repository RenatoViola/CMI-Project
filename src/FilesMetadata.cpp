#include "FilesMetadata.h"


//--------------------------------------------------------------
void FilesMetadata::setup() {
	currentFileName = "testeXml.xml";
	if (!xml.load("testeXml.xml")) {
		ofLogError() << "Couldn't load file:" << currentFileName << ";";
	}
}

//--------------------------------------------------------------
void FilesMetadata::load(string filename) {
	
	if (!xml.load(filename)) {
		ofLogError() << "Couldn't load file:" << filename << ";";
	}
	else {
		currentFileName = filename;
	}

}

//--------------------------------------------------------------
void FilesMetadata::save(string filename) {
	if (!xml.save(filename)) {
		ofLogError() << "Couldn't save file:" << filename << ";";
	}
}

//--------------------------------------------------------------
vector<string> FilesMetadata::getFileTags() {
	vector<string> tags;


	auto xmlTags = xml.find("//tags/tagName");
	for (auto & tag : xmlTags) {
		tags.push_back(tag.getValue());
	}

	return tags;

}

//--------------------------------------------------------------
void FilesMetadata::createFile(string fileName, vector<string> tags) {
	xml.clear();

	auto file = xml.appendChild("File");

	// Set the FileName
	file.appendChild("FileName").set(fileName);
	

	// Adds file tags
	auto xmlTags = file.appendChild("tags");
	for (auto tag : tags) {
		auto tagName = xmlTags.appendChild("TagName");
		tagName.set(tag);
	}

	// add Luminance
	file.appendChild("luminance");
	
	// add color
	file.appendChild("color");

	// add Number of faces
	file.appendChild("NFaces");

	xml.save(fileName);
}

//--------------------------------------------------------------
void FilesMetadata::addTag(string tag) {
	auto tags = xml.findFirst("//File/tags");
	tags.appendChild("TagName").set(tag);
}