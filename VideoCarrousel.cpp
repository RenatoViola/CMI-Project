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

void VideoCarrousel::draw(int numFilesToShow, int width, int height, int gridSpacing) {

	// Calculate the total width of the videos including spacing
	int totalWidth = numFilesToShow * width + (numFilesToShow - 1) * gridSpacing;

	// Calculate the starting x position so the selected video is in the center
	int startX = (ofGetWidth() - totalWidth) / 2;

	int yPosition = (ofGetHeight() - height) / 2; // Vertically center
	yPosition += height; // ...

	ofSetColor(ofColor::black);
	ofDrawBitmapString("VIDEOS", startX + totalWidth / 2 - 18, yPosition - 50);

	ofSetColor(ofColor::white);
	for (int i = 0; i < numFilesToShow; i++) {
		// Find the index of the video to display
		int displayIndex = (current - (numFilesToShow / 2) + i + videos.size()) % videos.size();

		// Calculate x and y position for the current video
		int xPosition = startX + i * (width + gridSpacing);
		

		// Draw the video at the calculated position
		if (displayIndex == current)
		{
			videos[displayIndex].draw(xPosition - 16, yPosition - 8, width + 32, height + 16);
		}
		else {
			videos[displayIndex].draw(xPosition, yPosition, width, height); // Draw each video at (xPosition, 0)
		}
	}
}

void VideoCarrousel::next() {
	if (dir.size() > 0) {
		videos[current].setPaused(true);
		current++;
		current %= dir.size();
		videos[current].play();
	}
}

void VideoCarrousel::previous() {
	if (dir.size() > 0) {
		videos[current].setPaused(true);
		current--;
		current %= dir.size();
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