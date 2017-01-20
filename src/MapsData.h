/*
 * Copyright 2016-2017 Raefaldhi Amartya Junior <raefaldhiamartya@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#ifndef MAPSDATA_H
#define MAPSDATA_H

#include <vector>

#include <Handler.h>
#include <UrlRequest.h>

class MapsData_Listener;

enum {
	MAPDATA_UPDATE = 'mdud'
};

struct MapsVector {
	int	width;
	int	height;

	float scale;

	float	longitude;
	float	latitude;
	float	zoom;
	float	bearing;
	float	pitch;
};

class MapsData {
protected:
	MapsData();
	~MapsData();
	
	static BMallocIO* 				data;
	static std::vector<BHandler*> 	handler;
public:
	static void Initialize(int, int);
	static void Retrieve();

	static void SetLongitude(float);
	static void SetLatitude(float);
	static void SetZoom(float);

	static BMallocIO* Get();
	static void AddHandler(BHandler*);
	
	static MapsVector GetVector();
private:
	static MapsVector mapsVector;

	static thread_id 			thread;
	static BUrlRequest*			request;
	static MapsData_Listener*	listener;

	static BString baseUrl;
};

#endif // MAPSDATA_H
