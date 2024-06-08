#include "MediaCircle.h"
#include <ImageMedia.h>
#include <VideoMedia.h>
#include <Metadata.h>

void MediaCircle::setup(vector<string>& paths, int radius, int width, int height)
{
	this->radius = radius;
	this->width = width;
	this->height = height;

	items.reserve(paths.size());

	for (string path : paths) {
		unique_ptr<Media> item;
		if (Media::isImage(path))
			item = make_unique<ImageMedia>();
		else
			item = make_unique<VideoMedia>();

		item->load(path);
		item->update();
		items.push_back(move(item));
	}

	/* Calculate image pos in circle */
	angleInRadians = 2 * PI / this->items.size();

	// Setup bounding rectangles
	updateBoundingBoxes();
}

void MediaCircle::update()
{
	for (int i = 0; i < items.size(); i++)
	{
		items[i].get()->update();
	}
}

void MediaCircle::draw()
{
	for (int i = 0; i < items.size(); i++)
	{
		Media* m = items[i].get();
		const ofRectangle& rect = boundingBoxes[i];
		if (i + 1 <= versions.size())
		{
			ofRectangle border;
			border.set(rect.x - 10, rect.y - 10, rect.width + 20, rect.height + 20);
			ofSetColor(ofColor::paleGreen);
			ofDrawRectRounded(border, 10);

			auto t = versions[i];
			stringstream caption;
			caption << "ALT. " << get<0>(t) << " | " << get<2>(t) << endl;
			ofSetColor(ofColor::black);
			ofDrawBitmapString(caption.str(), rect.x, rect.y);
			ofSetColor(ofColor::white);
		}
		m->draw(rect.x, rect.y, rect.width, rect.height);
	}
}


void MediaCircle::exit()
{
	items.clear();
}


void MediaCircle::mouseReleased(int x, int y, int button) {
	for (int i = 0; i < boundingBoxes.size(); i++) {
		if (boundingBoxes[i].inside(x, y)) {
			selectedMedia = i;
			ofLogNotice() << "Media item clicked: " << items[selectedMedia]->getFilePath();
			ofNotifyEvent(clickedOnItem, this);

			break;
		}
	}
}

string MediaCircle::getCurrentFilePath() {
	return items[selectedMedia].get()->getFilePath();
}

int MediaCircle::getCurrentIndex() {
	return selectedMedia;
}

void MediaCircle::updateBoundingBoxes() {
	int centerX = ofGetWidth() / 2;
	int centerY = ofGetHeight() / 2;

	boundingBoxes.clear();
	for (int i = 0; i < items.size(); i++)
	{
		double angle = angleInRadians * i;

		float x = centerX + radius * cos(angle);
		float y = centerY + radius * sin(angle);

		x -= width / 2;
		y -= height / 2;

		ofRectangle rect(x, y, width, height);
		boundingBoxes.push_back(rect);
	}
}

void MediaCircle::setVersions(const string& filePath) {
	this->versions = Metadata::getVersionedDates(filePath);
}