#ifndef MAPSWINDOW_H
#define MAPSWINDOW_H

#include <Window.h>

#include "MapsView.h"
#include "VirtualScroller.h"

class MapsWindow : public BWindow {
public:
	MapsWindow();
	~MapsWindow();
private:
	VirtualScroller* virtualScroller;
};

#endif // MAPSWINDOW_H
