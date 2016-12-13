#ifndef MAPSVIEW_H
#define MAPSVIEW_H

#include <View.h>
#include <Bitmap.h>
#include <UrlRequest.h>
#include <UrlProtocolRoster.h>
#include <UrlProtocolListener.h>

#define REQUEST_FINISHED 0x0001

class MapsViewListener : public BUrlProtocolListener {
private:
	BHandler* target;
public:
	BMallocIO *bitmapData;
	
	MapsViewListener(BHandler* handler);
 	virtual ~MapsViewListener();
	
	virtual void DataReceived(BUrlRequest* caller, const char* data, off_t position, ssize_t size);
	virtual void RequestCompleted(BUrlRequest* caller, bool success);
};

class MapsView : public BView {
public:	
	MapsView(float, float, float, float, float, int, int);
	virtual ~MapsView();

	void Refresh();

	virtual void Draw(BRect updateRect);
	virtual void MouseDown(BPoint where);
	virtual void MouseUp(BPoint where);
	virtual void MessageReceived(BMessage*);

	void SetLongitude(float);
	void SetLatitude(float);
	void SetZoom(float);
private:
	float	longitude;
	float	latitude;
	float	zoom;
	float	bearing;
	float	pitch;
	int		width;
	int		height;
	
	float	divisor;

	BBitmap* bitmap;
	
	BUrlRequest*		request;
	MapsViewListener*	listener;
	thread_id 			thread;

	bool IsMouseDown;
	BPoint pastPoint;
};

#endif // MAPSVIEW_H
