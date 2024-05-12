#include "Carrousel.h"


void Carrousel::setup(vector<unique_ptr<Media>>&& items, int verticalPos, string label) {

	this->items = move(items);
	this->label = label;

	startY = verticalPos;

	fullCarrousel.set(startX, startY, totalWidth, height);
	int xPos = startX + 2 * space;
	selectedFile.set(xPos - currentWidth / 2, startY - currentHeight / 2, width + currentWidth, height + currentHeight);
}

void Carrousel::exit() {
//	items.clear();
}

void Carrousel::draw() {
	// Calculate progressive step 
	step = ofLerp(step, space, 0.05);

	ofSetColor(ofColor::black);
	ofDrawBitmapString(label, startX + totalWidth / 2 - 18, startY - 50);

	ofSetColor(ofColor::white);
	for (int i = 0; i < numFilesToShow; i++) {
		// Find the index of the video to display
		int displayIndex = (current - (numFilesToShow / 2) + i + items.size()) % items.size();

		// Calculate x and y position for the current video
		int xPos = startX + i * space;
		if (loadFromTheRight)
		{
			xPos += (space - step);
		}
		else {
			xPos -= (space - step);
		}

		// Draw the video at the calculated position
		if (displayIndex == current)
		{
			currentWidth = ofLerp(currentWidth, 40, 0.05);
			currentHeight = ofLerp(currentHeight, 30, 0.05);

			items[displayIndex]->draw(xPos - currentWidth / 2, startY - currentHeight / 2, width + currentWidth, height + currentHeight);
		}
		else {
			items[displayIndex]->draw(xPos, startY, width, height); // Draw each video at (xPosition, 0)
		}
	}
}

void Carrousel::update() {
	for (auto& item : items) {
		item->update();
	}
}

void Carrousel::mousePressed(int x, int y, int button) {
	lastX = x;
}

void Carrousel::mouseReleased(int x, int y, int button) {

	if (selectedFile.inside(x, y))
	{
		ofNotifyEvent(clickedOnSelected, this);
		return;
	}

	if (!fullCarrousel.inside(x, y)) return;

	int deltaX = x - lastX;

	int threshold = 50;

	if (deltaX < -threshold) { // Dragged from right to left
		next();
	}
	else if (deltaX > threshold) { // Dragged from left to right
		previous();
	}
}

void Carrousel::next() {
	if (items.size() > 0) {
		++current %= items.size();
		step = 0;
		loadFromTheRight = true;
	}
}

void Carrousel::previous() {
	if (items.size() > 0) {
		if (--current == -1)
			current = items.size() - 1;
		step = 0;
		loadFromTheRight = false;
	}
}

Media* Carrousel::getCurrentMedia() {
	return items[current].get();
}