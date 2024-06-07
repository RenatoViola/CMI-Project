#include "MediaCircle.h"
#include <ImageMedia.h>
#include <VideoMedia.h>

void MediaCircle::setup(vector<string> paths)
{
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
//	ofSetColor(ofColor::white);
	for (int i = 0; i < items.size(); i++)
	{
		Media* m = items[i].get();
		const ofRectangle& rect = boundingBoxes[i];
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

		float x = centerX + CIRCLE_RADIUS * cos(angle);
		float y = centerY + CIRCLE_RADIUS * sin(angle);

		x -= MEDIA_WIDTH / 2;
		y -= MEDIA_HEIGHT / 2;

		ofRectangle rect(x, y, MEDIA_WIDTH, MEDIA_HEIGHT);
		boundingBoxes.push_back(rect);
	}
}