#ifndef MAPSVIEW_H
#define MAPSVIEW_H

#include <View.h>
#include <Bitmap.h>
#include <UrlRequest.h>
#include <UrlProtocolRoster.h>
#include <UrlProtocolListener.h>

#define REQUEST_SUCCESS 0x0001

class MapsViewListener : public BUrlProtocolListener {
private:
	BHandler* target;
public:
	BMallocIO *bitmapData;
	
	MapsViewListener(BHandler*);
 	virtual ~MapsViewListener();
	
	virtual void DataReceived(BUrlRequest*, const char*, off_t, ssize_t);
	virtual void RequestCompleted(BUrlRequest*, bool);
};

class MapsView : public BView {
public:	
	MapsView(float, float, float, float, float, int, int);
	virtual ~MapsView();

	void Refresh();

	virtual void Draw(BRect);
	virtual void MouseDown(BPoint);
	virtual void MouseUp(BPoint);
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
	
	float	scale;

	BBitmap* bitmap;
	
	BUrlRequest*		request;
	MapsViewListener*	listener;
	thread_id 			thread;

	bool IsMouseDown;
	BPoint pastPoint;
};

#endif // MAPSVIEW_H
