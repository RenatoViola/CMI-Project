#include "Metadata.h"


//--------------------------------------------------------------
void Metadata::load(string filename, ofXml XML, bool isImage) {
	
	const string PATH = (isImage ? "imagesXML/" : "videosXML/") + filename + ".xml";

	if (!XML.load(PATH)) {
		ofLogError() << "Couldn't load file: " << filename << "; " << "Will create one.";
		XML.clear();
		createFile(filename, XML, isImage);
		save(filename, XML, isImage);
	}
}

//--------------------------------------------------------------
void Metadata::save(string filename, ofXml XML, bool isImage) {
	const string PATH = (isImage ? "imagesXML/" : "videosXML/") + filename + ".xml";

	if (!XML.save(PATH)) {
		ofLogError() << "Couldn't save file:" << filename << ";";
	}
}

//--------------------------------------------------------------
vector<string> Metadata::getTags(ofXml XML) {
	vector<string> tags;

	auto xmlTags = XML.find("//TAGS/TAG");

	if (xmlTags.empty())
	{
		throw new exception("There are no tags for this file.");
	}

	for (auto & tag : xmlTags) {
		tags.push_back(tag.getValue());
	}

	return tags;
}

//--------------------------------------------------------------
void Metadata::createFile(string fileName, ofXml XML, bool isImage) {

	auto metadata = XML.appendChild("MEDIA");

	// Set the FileName
	metadata.appendChild("FILENAME").set(fileName);

	// Adds tags section
	auto xmlTags = metadata.appendChild("TAGS");

	// add Luminance
	metadata.appendChild("LUMINANCE");
	
	// add color
	auto color = metadata.appendChild("COLOR");
	color.appendChild("RED");
	color.appendChild("GREEN");
	color.appendChild("BLUE");

	// add Number of faces
	metadata.appendChild("NUM_FACES");

	metadata.appendChild("EDGE_DISTRIBUTION");
	
	metadata.appendChild("TEXTURE_CHARACTERISTICS");
	
	metadata.appendChild("OBJECT_OCURRENCES");
}