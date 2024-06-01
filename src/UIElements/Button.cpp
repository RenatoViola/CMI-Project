#include "Button.h"

void Button::setup(const std::string& iconPath, int radius, int x, int y)
{
	icon.load(iconPath);
	this->radius = radius;
	this->x = x;
	this->y = y;
}

void Button::draw()
{
	icon.draw(x, y, radius, radius);
}

void Button::mouseReleased(int x, int y, int button) {
	if (inside(x, y)) {
		ofNotifyEvent(clickedInside, this);
	}
}

bool Button::inside(int x, int y)
{
	return (glm::distance(glm::vec2(this->x + radius, this->y + radius), glm::vec2(x, y)) < radius);
}