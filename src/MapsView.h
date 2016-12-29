#ifndef MAPSVIEW_H
#define MAPSVIEW_H

#include <vector>

#include <View.h>
#include <Bitmap.h>
#include <UrlRequest.h>
#include <UrlProtocolRoster.h>
#include <UrlProtocolListener.h>

#include "VirtualScroller.h"

enum {
	M_MAPSVIEW_ON_FOCUS = 'mmct',
	M_MAPSVIEW_ATTACHEDTOWINDOW = 'matw'
};

class MapsView : public BView {
public:	
	MapsView();
	virtual ~MapsView();

	void AddHandler(BHandler*);

	virtual void Draw(BRect);
	virtual void MouseDown(BPoint);
	virtual void MouseUp(BPoint);
	virtual void MessageReceived(BMessage*);
private:
	BBitmap* bitmap;

	VirtualScroller* virtualScroller;

	bool IsMouseDown;
	BPoint pastPoint;
	
	std::vector<BHandler*> handler;
};

#endif // MAPSVIEW_H
