#include "ImageCarrousel.h"

void ImageCarrousel::setup(const std::string& path) {

	Carrousel::setup(path, true);
	dir.allowExt("jpg");
	dir.allowExt("png");
	dir.listDir(path);
	dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order

	//allocate the vector to have as many ofImages as files
	if (dir.size()) {
		images.assign(dir.size(), ofImage());
	}

	// you can now iterate through the files and load them into the ofImage vector
	for (int i = 0; i < (int)dir.size(); i++) {
		images[i].load(dir.getPath(i));
		Metadata::load(dir.getName(i), xml, true);
	}

	startY = horizontalMiddle - vHeight; // Top half of the screen
	fullCarrousel.set(startX, startY, totalWidth, vHeight);
	int xPos = startX + 2 * space;
	selectedFile.set(xPos - currentWidth / 2, startY - currentHeight / 2, vWidth + currentWidth, vHeight + currentHeight);
}


void ImageCarrousel::draw() {
	// Calculate progressive step 
	step = ofLerp(step, space, 0.05);

	ofSetColor(ofColor::black);
	ofDrawBitmapString("IMAGES", startX + totalWidth / 2 - 18, startY - 50);

	ofSetColor(ofColor::white);
	for (int i = 0; i < numFilesToShow; i++) {
		// Find the index of the video to display
		int displayIndex = (current - (numFilesToShow / 2) + i + images.size()) % images.size();

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

			images[displayIndex].draw(xPos - currentWidth / 2, startY - currentHeight / 2, vWidth + currentWidth, vHeight + currentHeight);
		}
		else {
			images[displayIndex].draw(xPos, startY, vWidth, vHeight);
		}
	}
}


void ImageCarrousel::displayCurrent() {
	float iWidth = images[current].getWidth(), iHeight = images[current].getHeight();

	float sWidth = ofGetWidth(), sHeight = ofGetHeight();

	ofSetColor(ofColor::black);
	ofDrawRectangle(0, 0, sWidth, sHeight);

	float scale = 1.0f;

	if (iWidth > sWidth || iHeight > sHeight)
		scale = std::min(sWidth / iWidth, sHeight / iHeight);

	float displayWidth = iWidth * scale, displayHeight = iHeight * scale;

	float xPos = (sWidth - displayWidth) / 2.0f, yPos = (sHeight - displayHeight) / 2.0f;

	ofSetColor(ofColor::white);
	images[current].draw(xPos, yPos, displayWidth, displayHeight);
	panel.draw();
}

void ImageCarrousel::next() {
	if (dir.size() > 0) {
		++current %= dir.size();
		step = 0;
		loadFromTheRight = true;
	}
}

void ImageCarrousel::previous() {
	if (dir.size() > 0) {
		if (--current == -1)
			current = dir.size() - 1;
		step = 0;
		loadFromTheRight = false;
	}
}