#include "VideoCarrousel.h"

void VideoCarrousel::setup(const std::string& path) {
	frameByframe = false;

	dir.allowExt("mp4"); // Filter for mp4 files
	dir.listDir(path); // List files in the directory
	dir.sort();

	// Resize the vector to match the number of video files
	videos.resize(dir.size());

	for (size_t i = 0; i < dir.size(); ++i) {
		// Load each video into an ofVideoPlayer object
		videos[i].load(dir.getPath(i));
	}

	current = 0;
	videos[current].setLoopState(OF_LOOP_NORMAL);
	videos[current].play();
}

void VideoCarrousel::update() {
	for (size_t i = 0; i < videos.size(); ++i) {
		videos[i].update(); // Update each video player
	}
}

void VideoCarrousel::drawAsList(int numFilesToShow, int width, int height, int gridSpacing) {

	// Calculate the total width of the videos including spacing
	int totalWidth = numFilesToShow * width + (numFilesToShow - 1) * gridSpacing;

	// Calculate the starting x position so the selected video is in the center
	int startX = (ofGetWidth() - totalWidth) / 2;

	int yPos = (ofGetHeight() - height) / 2; // Vertically center

	yPos += height; // ...

	ofSetColor(ofColor::black);
	ofDrawBitmapString("VIDEOS", startX + totalWidth / 2 - 18, yPos - 50);

	ofSetColor(ofColor::white);
	for (int i = 0; i < numFilesToShow; i++) {
		// Find the index of the video to display
		int displayIndex = (current - (numFilesToShow / 2) + i + videos.size()) % videos.size();

		// Calculate x and y position for the current video
		int xPos = startX + i * (width + gridSpacing);
		
		// Draw the video at the calculated position
		if (displayIndex == current)
		{
			videos[displayIndex].draw(xPos - 20, yPos - 15, width + 40, height + 30);
		}
		else {
			videos[displayIndex].draw(xPos, yPos, width, height); // Draw each video at (xPosition, 0)
		}
	}
}

void VideoCarrousel::drawAsRevolver(int radius, int width, int height) {
	int centerX = ofGetWidth() / 2;
	int centerY = ofGetHeight() / 2;

	float angleStep = TWO_PI / videos.size(); // The angle between each video

	for (size_t i = 0; i < videos.size(); ++i) {
		float angle = i * angleStep;

		// Position videos in a circular layout
		int xPosition = centerX + cos(angle) * radius;
		int yPosition = centerY + sin(angle) * radius;

		// Determine the size of the video, larger if it's the selected index
		int videoWidth = (i == current) ? width + 40 : width;
		int videoHeight = (i == current) ? height + 30 : height;

		// Adjust positions to draw from the center of each video
		xPosition -= videoWidth / 2;
		yPosition -= videoHeight / 2;

		// Draw the video
		videos[i].draw(xPosition, yPosition, videoWidth, videoHeight);
	}
}


void VideoCarrousel::displayCurrent() {
	float vWidth = videos[current].getWidth(), vHeight = videos[current].getHeight();

	float sWidth = ofGetWidth(), sHeight = ofGetHeight();

	ofSetColor(ofColor::black);
	ofDrawRectangle(0, 0, sWidth, sHeight);
	
	float scale = 1.0f;

	if (vWidth > sWidth || vHeight > sHeight)
		scale = std::min(sWidth / vWidth, sHeight / vHeight);

	float displayWidth = vWidth * scale, displayHeight = vHeight * scale;

	float xPos = (sWidth - displayWidth) / 2.0f, yPos = (sHeight - displayHeight) / 2.0f;

	ofSetColor(ofColor::white);
	videos[current].draw(xPos, yPos, displayWidth, displayHeight);
}

void VideoCarrousel::next() {
	if (dir.size() > 0) {
		videos[current].setPaused(true);
		++current %= dir.size();
		videos[current].play();
	}
}

void VideoCarrousel::previous() {
	if (dir.size() > 0) {
		videos[current].setPaused(true);
		if (--current == -1)
			current = dir.size() - 1;
		videos[current].play();
	}
}

void VideoCarrousel::pause() {
	frameByframe = !frameByframe;
	videos[current].setPaused(frameByframe);
}

void VideoCarrousel::nextFrame() {
	videos[current].nextFrame();
}

void VideoCarrousel::previousFrame() {
	videos[current].nextFrame();
}

void VideoCarrousel::firstFrame() {
	videos[current].firstFrame();
}