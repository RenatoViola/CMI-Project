#include "Carrousel.h"


void Carrousel::setup(vector<unique_ptr<Media>>&& items, bool isImageCarrousel, string label) {

	this->items = move(items);
	this->isImageCarrousel = isImageCarrousel;
	this->label = label;

	if (isImageCarrousel)
	{
		startY = horizontalMiddle - height; // Top half of the screen
	}
	else {
		startY = horizontalMiddle + height; // Bottom half of the screen
	}

	fullCarrousel.set(startX, startY, totalWidth, height);
	int xPos = startX + 2 * space;
	selectedFile.set(xPos - currentWidth / 2, startY - currentHeight / 2, width + currentWidth, height + currentHeight);

	ofAddListener(ofEvents().mousePressed, this, &Carrousel::mousePressed);
	ofAddListener(ofEvents().mouseReleased, this, &Carrousel::mouseReleased);
}

void Carrousel::exit() {
	ofRemoveListener(ofEvents().mousePressed, this, &Carrousel::mousePressed);
	ofRemoveListener(ofEvents().mouseReleased, this, &Carrousel::mouseReleased);

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

void Carrousel::mousePressed(ofMouseEventArgs& args) {
	lastX = args.x;
}

void Carrousel::mouseReleased(ofMouseEventArgs& args) {

	int x = args.x, y = args.y;

	if (selectedFile.inside(x, y))
	{
		int PAGE;
		if (isImageCarrousel)
		{
			PAGE = IMAGE_PAGE;
		}
		else {
			PAGE = VIDEO_PAGE;
		}
		ofNotifyEvent(onChangeScreen, PAGE, this);

		return;
	}

	if (!fullCarrousel.inside(x, y)) return;

	int deltaX = args.x - lastX;

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