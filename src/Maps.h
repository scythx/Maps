/*
 * Copyright 2016-2017 Raefaldhi Amartya Junior <raefaldhiamartya@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

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
