#ifndef MAPSWINDOW_H
#define MAPSWINDOW_H

#include <Window.h>
#include "MapsView.h"

class MapsWindow : public BWindow {
public:
	MapsWindow();
	~MapsWindow();
private:
	MapsView* mapsView;
};

#endif // MAPSWINDOW_H
