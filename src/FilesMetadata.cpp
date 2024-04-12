#include "FilesMetadata.h"

const string PATH = "";

//--------------------------------------------------------------
void FilesMetadata::setup() {
	currentFileName = PATH + "testeXml.xml";
	if (!xml.load("testeXml.xml")) {
		ofLogError() << "Couldn't load file:" << currentFileName << ";";
	}
}

//--------------------------------------------------------------
void FilesMetadata::load(string filename) {
	
	if (!xml.load(PATH + filename)) {
		ofLogError() << "Couldn't load file:" << filename << ";";
	}
	else {
		currentFileName = filename;
	}

}

//--------------------------------------------------------------
void FilesMetadata::save(string filename) {
	if (!xml.save(PATH + filename)) {
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

	auto metadata = xml.appendChild("File");

	// Set the FileName
	metadata.appendChild("FileName").set(fileName);

	// Adds file tags
	auto xmlTags = metadata.appendChild("tags");
	for (auto tag : tags) {
		auto tagName = xmlTags.appendChild("TagName");
		tagName.set(tag);
	}

	// add Luminance
	metadata.appendChild("luminance");
	
	// add color
	auto color = metadata.appendChild("color");
	color.appendChild("red");
	color.appendChild("green");
	color.appendChild("blue");

	// add Number of faces
	metadata.appendChild("num_faces");

	metadata.appendChild("edge_distribution");
	
	metadata.appendChild("texture_characteristics");
	
	metadata.appendChild("object_ocurrences");



	xml.save(PATH + fileName);
}

//--------------------------------------------------------------
void FilesMetadata::addTag(string tag) {
	auto tags = xml.findFirst("//File/tags");
	tags.appendChild("TagName").set(tag);
}