#include "Button.h"

void Button::setup(const std::string& iconPath, int radius, int x, int y)
{
	icon.load(iconPath);
	this->radius = radius;
	this->x = x;
	this->y = y;
}

void Button::update()
{
	uint64_t currentTime = ofGetElapsedTimeMillis();
	uint64_t elapsedTime = currentTime - startTime;

	if (elapsedTime < 1000) { // Fully opaque for the first 500 milliseconds (0.5 seconds)
		fadeAlpha = 255.0f;
	}
	else if (elapsedTime < 2000) { // Fade out over the next 1000 milliseconds (1 second)
		float fadeTime = elapsedTime - 1000;
		fadeAlpha = ofLerp(255.0f, 0.0f, fadeTime / 1000.0f);
	}
	else {
		fadeAlpha = 0.0f; // After 1.5 seconds, fully transparent
	}
}

void Button::draw()
{
	icon.draw(x, y, radius, radius);
}

void Button::timedDraw()
{
	if (fadeAlpha > 0.0f) { // Only draw if still visible
		ofSetColor(255, 255, 255, fadeAlpha); // Set the color with alpha
		icon.draw(x, y, radius, radius);
	}
}

void Button::loadIcon(const std::string& iconPath)
{
	icon.load(iconPath);
	startTime = ofGetElapsedTimeMillis(); // Initialize the start time
	fadeAlpha = 255.0f;                   // Initially, fully opaque
}

void Button::mouseReleased(int x, int y, int button) {
	if (inside(x, y)) {
		ofNotifyEvent(clickedInside, this);
	}
}

bool Button::inside(int x, int y)
{
	return glm::distance(glm::vec2(this->x + radius, this->y + radius), glm::vec2(x, y)) < radius;
}