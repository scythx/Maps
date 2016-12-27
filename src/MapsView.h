#ifndef MAPSVIEW_H
#define MAPSVIEW_H

#include <View.h>
#include <Bitmap.h>
#include <UrlRequest.h>
#include <UrlProtocolRoster.h>
#include <UrlProtocolListener.h>

#include "VirtualScroller.h"

class MapsView : public BView {
public:	
	MapsView(float, float, float, float, float, int, int);
	virtual ~MapsView();

	virtual void Draw(BRect);
	virtual void MouseDown(BPoint);
	virtual void MouseUp(BPoint);
	virtual void MessageReceived(BMessage*);
private:
	BBitmap* bitmap;

	VirtualScroller* virtualScroller;

	bool IsMouseDown;
	BPoint pastPoint;
};

#endif // MAPSVIEW_H
