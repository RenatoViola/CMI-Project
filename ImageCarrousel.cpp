#include "ImageCarrousel.h"

void ImageCarrousel::setup(const std::string& path) {
	dir.allowExt("jpg");
	dir.allowExt("png");
	dir.listDir(path);
	dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order

	//allocate the vector to have as many ofImages as files
	if (dir.size()) {
		images.assign(dir.size(), ofImage());
	}

	// you can now iterate through the files and load them into the ofImage vector
	for (int i = 0; i < (int)dir.size(); i++) 
		images[i].load(dir.getPath(i));
	
	current = 0;
}

void ImageCarrousel::drawAsList(int numFilesToShow, int width, int height, int gridSpacing) {

	// Calculate the total width of the videos including spacing
	int totalWidth = numFilesToShow * width + (numFilesToShow - 1) * gridSpacing;

	// Calculate the starting x position so the selected video is in the center
	int startX = (ofGetWidth() - totalWidth) / 2;

	int yPos = (ofGetHeight() - height) / 2; // Vertically center

	yPos -= height; // ...

	ofSetColor(ofColor::black);
	ofDrawBitmapString("IMAGES", startX + totalWidth / 2 - 18, yPos - 50);

	ofSetColor(ofColor::white);
	for (int i = 0; i < numFilesToShow; i++) {
		// Find the index of the video to display
		int displayIndex = (current - (numFilesToShow / 2) + i + images.size()) % images.size();

		// Calculate x and y position for the current video
		int xPos = startX + i * (width + gridSpacing);

		// Draw the video at the calculated position
		if (displayIndex == current)
		{
			images[displayIndex].draw(xPos - 20, yPos - 15, width + 40, height + 30);
		}
		else {
			images[displayIndex].draw(xPos, yPos, width, height); // Draw each video at (xPosition, 0)
		}
	}
}

void ImageCarrousel::drawAsRevolver(int radius, int width, int height) {
	int centerX = ofGetWidth() / 2;
	int centerY = ofGetHeight() / 2;

	float angleStep = TWO_PI / images.size(); // The angle between each video

	for (size_t i = 0; i < images.size(); ++i) {
		float angle = i * angleStep;

		// Position videos in a circular layout
		int xPosition = centerX + cos(angle) * radius;
		int yPosition = centerY + sin(angle) * radius;

		// Determine the size of the video, larger if it's the selected index
		int imageWidth = (i == current) ? width + 40 : width;
		int imageHeight = (i == current) ? height + 30 : height;

		// Adjust positions to draw from the center of each video
		xPosition -= imageWidth / 2;
		yPosition -= imageHeight / 2;

		// Draw the video
		images[i].draw(xPosition, yPosition, imageWidth, imageHeight);
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
}

void ImageCarrousel::next() {
	if (dir.size() > 0) {
		++current %= dir.size();
	}
}

void ImageCarrousel::previous() {
	if (dir.size() > 0) {
		if (--current == -1)
			current = dir.size() - 1;
	}
}