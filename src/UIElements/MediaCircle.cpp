#include "MediaCircle.h"

void MediaCircle::setup(vector<unique_ptr<Media>>&& items)
{
	this->items = move(items);

	/* Calculate image pos in circle */
	angleInRadians = 2 * PI / this->items.size();
	cout << angleInRadians << endl;


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
	int centerX = ofGetWidth() / 2;
	int centerY = ofGetHeight() / 2;

	ofSetColor(ofColor::white);
	for (int i = 0; i < items.size(); i++)
	{
		double angle = angleInRadians * i;

		Media* m = items[i].get();
		float x = centerX + CIRCLE_RADIUS * cos(angle);
		float y = centerY + CIRCLE_RADIUS * sin(angle);

		x -= MEDIA_WIDTH / 2;
		y -= MEDIA_HEIGHT / 2;

		m->draw(x, y, MEDIA_WIDTH, MEDIA_HEIGHT);
	}
}


void MediaCircle::exit()
{
	items.clear();
}

Media* MediaCircle::getSelectedMedia()
{
	if (selectedMedia >= items.size()) return NULL;

	return items[selectedMedia].get();
}