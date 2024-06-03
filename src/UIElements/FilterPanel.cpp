#include "FilterPanel.h"

void FilterPanel::setup(const string& filePath)
{
	this->filePath = filePath;
	isImageFile = Media::isImage(filePath);

	panelVisibility = true;

	gui.setup("GUI", "gui.xml", static_cast<float>(ofGetWidth() - 220), 20.0f);

	gui.add(filtersLabel.setup("FILTERS", ""));
	if (isImageFile)
	{
		gui.add(asciiFilter.setup("ASCII", false));
	}
	
	gui.add(invertColorFilter.setup("Inverted Colors", false));
	gui.add(edgeFilter.setup("Edge Filter", false));
	
	gui.add(spaceLabel.setup("", ""));
	gui.add(changesLabel.setup("CHANGES", ""));
	gui.add(saveBtn.setup("SAVE"));

	saveBtn.addListener(this, &FilterPanel::saveButtonPressed);
}

void FilterPanel::draw()
{
	// auto draw?
	// should the gui control hiding?
	if (panelVisibility) {
		gui.draw();
	}
}


void FilterPanel::collapseButton()
{
	panelVisibility = !panelVisibility;
}

void FilterPanel::saveButtonPressed() {
	ofXml xml;
	string savePath = "versions/" + filePath + ".xml";
	bool fileExists = xml.load(savePath);

	ofXml root;
	if (fileExists) {
		root = xml.getChild("METADATA");
	}
	else {
		root = xml.appendChild("METADATA");
		root.appendChild("FILETYPE").set(isImageFile ? "IMAGE" : "VIDEO");
		root.appendChild("FILENAME").set(filePath);
	}

	auto versionsNode = root.findFirst("VERSIONS");
	if (!versionsNode) {
		versionsNode = root.appendChild("VERSIONS");
	}

	// Determine the next unique version ID
	int nextVersionID = 1;
	auto versionNodes = versionsNode.getChildren("VERSION");
	for (auto& versionNode : versionNodes) {
		int versionID = versionNode.getAttribute("ID").getIntValue();
		if (versionID >= nextVersionID) {
			nextVersionID = versionID + 1;
		}
	}

	auto versionNode = versionsNode.appendChild("VERSION");
	versionNode.setAttribute("ID", nextVersionID);
	versionNode.appendChild("DATE").set(ofGetTimestampString("%Y-%m-%d %H:%M:%S"));
	auto filtersNode = versionNode.appendChild("FILTERS");

	filtersNode.appendChild("INVERTED_COLORS").set(static_cast<int>(invertColorFilter));
	filtersNode.appendChild("EDGE_FILTER").set(static_cast<int>(edgeFilter));

	if (isImageFile)
	{
		filtersNode.appendChild("ASCII_FILTER").set(static_cast<int>(asciiFilter));
	}
	
	if (!xml.save(savePath)) {
		ofLogError() << "Couldn't save file: " << savePath;
	}
	else {
		ofLogNotice() << "Saved file: " << savePath << " with version ID: " << nextVersionID;
	}
}

void FilterPanel::loadVersionInfo(int versionID) {
	ofXml xml;
	string loadPath = "versions/" + filePath + ".xml";

	if (!xml.load(loadPath)) {
		ofLogError() << "Couldn't load file: " << loadPath;
		return;
	}

	auto versionNode = xml.findFirst("//VERSION[@ID='" + ofToString(versionID) + "']");
	if (!versionNode) {
		ofLogError() << "No version with ID " << versionID << " found in file: " << loadPath;
		return;
	}
	
	auto filtersNode = versionNode.findFirst("FILTERS");
	if (!filtersNode) {
		ofLogError() << "No filter data found in version with ID " << versionID << " in file: " << loadPath;
		return;
	}

	invertColorFilter = filtersNode.getChild("INVERTED_COLORS").getIntValue();
	edgeFilter = filtersNode.getChild("EDGE_FILTER").getIntValue();

	if (isImageFile)
	{
		asciiFilter = filtersNode.getChild("ASCII_FILTER").getIntValue();
	}
	
	ofLogNotice() << "Loaded filter settings from version ID: " << versionID << " in file: " << loadPath;
}

void FilterPanel::exit() {
	saveBtn.removeListener(this, &FilterPanel::saveButtonPressed);
	gui.clear();
}
