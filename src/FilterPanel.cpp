#include "FilterPanel.h"

void FilterPanel::setup()
{
	panelVisibility = true;

	// setup panel

	// we add this listener before setting up so the initial circle resolution is correct

	gui.setup(); // most of the time you don't need a name
	gui.add(invertColorFilter.setup("Inverted Colors", false));
	gui.add(asciiFilter.setup("ASCII", false));
	gui.add(color.setup("color", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
}

void FilterPanel::update()
{
}

void FilterPanel::draw()
{
	// auto draw?
	// should the gui control hiding?
	if (panelVisibility) {
		gui.draw();
	}
}


void FilterPanel::colapseButton()
{
	panelVisibility = !panelVisibility;
}
