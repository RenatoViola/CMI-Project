#include "MediaCircle.h"


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
		item->getThumbnail();
		items.push_back(move(item));
	}

	/* Calculate image pos in circle */
	angleInRadians = 2 * PI / this->items.size();

	// Setup bounding rectangles
	updateBoundingBoxes();

	myFont.load("JuliusSansOne-Regular.ttf", 11);
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
		stringstream caption;
		ofRectangle border;
		border.set(rect.x - 5, rect.y - 5, rect.width + 10, rect.height + 10);
		
		if (i + 1 <= versions.size())
		{
			auto t = versions[i];
			caption << "ALT. " << get<0>(t) << " | " << get<2>(t) << endl;
			ofSetColor(ofColor::paleGreen);
		}
		else
		{
			caption << (Media::isImage(m->getFilePath()) ? "IMAGE" : "VIDEO") << endl;
			ofSetColor(ofColor::lightCyan);
		}

		ofDrawRectRounded(border, 10);
		float textWidth = myFont.stringWidth(caption.str());
		ofSetColor(ofColor::black);
		myFont.drawString(caption.str(), rect.x + (rect.width - textWidth) / 2.0f, rect.y - 10);
		ofSetColor(ofColor::white);

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