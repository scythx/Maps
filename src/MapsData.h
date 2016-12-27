#ifndef MAPSDATA_H
#define MAPSDATA_H

#include <Handler.h>
enum {
	MAPDATA_UPDATE = 'mdud'
};

struct Vector2 {
	float lon;
	float lat;
};

class MapsData {
private:
	MapsData();
public:
	static void SetLongitude(float);
	static void SetLatitude(float);
	static void SetZoom(float);
	static void Retrieve();
	static BMallocIO* Get();
	static void AddHandler(BHandler*);
	
	static float GetScale();
	static Vector2 GetCoords();	
};

#endif // MAPSDATA_H
