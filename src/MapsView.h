#ifndef MAPSVIEW_H
#define MAPSVIEW_H

#include <View.h>

class MapsTile : public BView {
	bool panning;
	BPoint syncval;
public:
	MapsTile();
	~MapsTile();

	virtual void MouseDown(BPoint);
	virtual void MouseUp(BPoint);
	virtual void MouseMoved(BPoint, uint32, const BMessage*);
};

class MapsView : public BView {
public:	
	MapsTile *tile;
	MapsView();
	virtual ~MapsView();
};

#endif // MAPSVIEW_H
