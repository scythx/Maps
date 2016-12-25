#ifndef MAPS_H
#define MAPS_H

#include <Application.h>

#include "MapsWindow.h"

class Maps : public BApplication {
public:
	Maps();
	~Maps();	
private:
	MapsWindow* mapsWindow;
};

#endif // MAPS_H
