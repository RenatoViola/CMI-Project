#include "VideoCarrousel.h"

void VideoCarrousel::setup(const std::string& path) {
	
	dir.allowExt("mp4"); // Filter for mp4 files
	dir.listDir(path); // List files in the directory
	dir.sort();

	// Resize the vector to match the number of video files
	videos.resize(dir.size());

	for (size_t i = 0; i < dir.size(); ++i) {
		// Load each video into an ofVideoPlayer object
		videos[i].load(dir.getPath(i));
		Metadata::load(dir.getName(i), xml, false);
	}

	videos[current].setLoopState(OF_LOOP_NORMAL);
	videos[current].play();

	startY = horizontalMiddle + vHeight; // Bottom half of the screen

	fullCarrousel.set(startX, startY, totalWidth, vHeight);

	selectedFile.set(xPos - currentWidth / 2, startY - currentHeight / 2, vWidth + currentWidth, vHeight + currentHeight);

	ofAddListener(ofEvents().mousePressed, this, &VideoCarrousel::mousePressed);
	ofAddListener(ofEvents().mouseReleased, this, &VideoCarrousel::mouseReleased);
}

void VideoCarrousel::exit() {
	ofRemoveListener(ofEvents().mousePressed, this, &VideoCarrousel::mousePressed);
	ofRemoveListener(ofEvents().mouseReleased, this, &VideoCarrousel::mouseReleased);
}

void VideoCarrousel::mousePressed(ofMouseEventArgs& args) {
	lastX = args.x;  // Initialize lastX when the mouse is first pressed.
}


void VideoCarrousel::mouseReleased(ofMouseEventArgs& args) {

	if (!fullCarrousel.inside(args.x, args.y)) return; // should only drag on the bottom half of the screen

	int deltaX = args.x - lastX; // Calculate the change in x position

	// Threshold for dragging sensitivity
	int threshold = 50;

	if (deltaX < -threshold) { // Dragged right 2 left
		next();
	}
	else if (deltaX > threshold) { // Dragged left 2 right
		previous();
	}
}

void VideoCarrousel::update() {
	for (size_t i = 0; i < videos.size(); ++i) {
		videos[i].update(); // Update each video player
	}
}

void VideoCarrousel::draw() {
	// Calculate progressive step 
	step = ofLerp(step, space,0.05);

	ofSetColor(ofColor::black);
	ofDrawBitmapString("VIDEOS", startX + totalWidth / 2 - 18, startY - 50);

	ofSetColor(ofColor::white);
	for (int i = 0; i < numFilesToShow; i++) {
		// Find the index of the video to display
		int displayIndex = (current - (numFilesToShow / 2) + i + videos.size()) % videos.size();

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

			videos[displayIndex].draw(xPos - currentWidth / 2, startY - currentHeight / 2, vWidth + currentWidth, vHeight + currentHeight);
		}
		else {
			videos[displayIndex].draw(xPos, startY, vWidth, vHeight); // Draw each video at (xPosition, 0)
		}
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
		step = 0;
		loadFromTheRight = true;
	}
}

void VideoCarrousel::previous() {
	if (dir.size() > 0) {
		videos[current].setPaused(true);
		if (--current == -1)
			current = dir.size() - 1;
		videos[current].play();
		step = 0;
		loadFromTheRight = false;
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