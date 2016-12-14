#include "MapsView.h"

#include <stdio.h>
#include <Messenger.h>
#include <TranslationUtils.h>

#include <math.h>

static BString baseUrl("https://api.mapbox.com/styles/v1/mapbox/streets-v8/static/%f,%f,%f,%f,%f/%dx%d?access_token=pk.eyJ1IjoicmFlZmFsZGhpYSIsImEiOiJjaXdnN3J0YTkwMTV1MnVraXgzNGowbTBuIn0.9RYCJF1sfuUD86QRuBItYw&attribution=false&logo=false");

MapsView::MapsView(float _longitude, float _latitude, float _zoom, 
				float _bearing, float _pitch, int _width, int _height) : BView("mapsView", B_WILL_DRAW) {
	bitmap		= NULL;				
	
	longitude	= _longitude;
	latitude	= _latitude;
	zoom		= _zoom;
	bearing		= _bearing;
	pitch		= _pitch;
	width		= _width;
	height		= _height;
	
	request		= NULL;
}

MapsView::~MapsView() {

}

void MapsView::Refresh() {
	if (request != NULL)
	{
		request->Stop();
		kill_thread(thread);
		
		delete listener->bitmapData;
		delete listener;
		delete request;
		
		listener	= NULL;
		request		= NULL;
	}
	BString dataUrl;
	dataUrl.SetToFormat(baseUrl.String(), longitude, latitude, zoom, bearing, pitch, width, height);
	
	listener = new MapsViewListener(this);
	request = BUrlProtocolRoster::MakeRequest(BUrl(dataUrl.String()), listener);

	thread = request->Run();
}

void MapsView::SetLongitude(float _longitude) {
	if (_longitude < -180.0) {
		longitude = 360.0  + _longitude;
	}
	else if (_longitude > 180.0) {
		longitude = -360.0  + _longitude;
	}
	else {
		longitude = _longitude;
	}
}

void MapsView::SetLatitude(float _latitude) {
	if (_latitude < -90.0) {
		latitude = -90.0;
	}
	else if (_latitude > 90.0) {
		latitude = 90.0;
	}
	else {
		latitude = _latitude;
	}
}

void MapsView::SetZoom(float _zoom) {
	zoom = _zoom;
	scale = 1.5 * pow(2.0, zoom);
}

void MapsView::Draw(BRect updateRect){
	if (bitmap != NULL) {
		SetViewBitmap(bitmap);
	}
}

void MapsView::MouseDown(BPoint where) {
	uint32 mouseClicked;
	GetMouse(NULL, &mouseClicked, false);
	if (mouseClicked == B_PRIMARY_MOUSE_BUTTON) {
		IsMouseDown = true;
		pastPoint = where;
	}	
}

void MapsView::MouseUp(BPoint where) {
	if (IsMouseDown) {
		IsMouseDown = false;

		if (pastPoint.x < where.x) {
			SetLongitude(longitude - ((where.x - pastPoint.x) / scale));
		}
		else if (pastPoint.x > where.x) {
			SetLongitude(longitude + ((pastPoint.x - where.x) / scale));
		}
		if (pastPoint.y < where.y) {
			SetLatitude(latitude + ((where.y - pastPoint.y) / scale));
		}
		else if (pastPoint.y > where.y) {
			SetLatitude(latitude - ((pastPoint.y - where.y) / scale));
		}
		Refresh();
	}
}

MapsViewListener::MapsViewListener(BHandler* handler) : BUrlProtocolListener(), target(handler) {
	bitmapData = new BMallocIO();
}

MapsViewListener::~MapsViewListener() {
 					
}
 	
void MapsViewListener::DataReceived(BUrlRequest* caller, const char* data, off_t position, ssize_t size) {
		bitmapData->WriteAt(position, data, size);
}

void MapsViewListener::RequestCompleted(BUrlRequest* caller, bool success) {
	if (success) {
		BMessenger messenger(target);
		messenger.SendMessage(new BMessage(REQUEST_SUCCESS));
	}
}

void MapsView::MessageReceived(BMessage* message) {
	switch (message->what) {
		case REQUEST_SUCCESS: {
			delete bitmap;

			bitmap = BTranslationUtils::GetBitmap(listener->bitmapData);
			Invalidate();

			break;
		}
		default: {
			BView::MessageReceived(message);
			break;
		}
	}
}
