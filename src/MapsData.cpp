#include "MapsData.h"

#include <vector>

#include <Messenger.h>
#include <UrlRequest.h>
#include <UrlProtocolRoster.h>
#include <UrlProtocolListener.h>

static BString baseUrl("https://api.mapbox.com/styles/v1/mapbox/streets-v8/static/%f,%f,%f,%f,%f/%dx%d?access_token=pk.eyJ1IjoicmFlZmFsZGhpYSIsImEiOiJjaXdnN3J0YTkwMTV1MnVraXgzNGowbTBuIn0.9RYCJF1sfuUD86QRuBItYw&attribution=false&logo=false");

static float 	longitude 	= 0.0f;
static float 	latitude 	= 0.0f;
static float 	zoom 		= 0.0f;
static float 	bearing		= 0.0f;
static float 	pitch		= 0.0f;
static int		width		= 400;
static	int		height		= 400;
	
static float 	scale		= 1.5f;

static std::vector<BHandler*> 	handler;
static BMallocIO*				data = NULL;

class MapsData_Listener : public BUrlProtocolListener {
public:
	MapsData_Listener() {
		if (data != NULL) {
			delete data;
			data = NULL;
		}
		data = new BMallocIO();
	}
	virtual void DataReceived(BUrlRequest* caller, const char* _data, off_t position, ssize_t size) {
		data->WriteAt(position, _data, size);
	}
	virtual void RequestCompleted(BUrlRequest* caller, bool success) {
		if (success) {
			for (std::vector<BHandler*>::iterator it = handler.begin(); it != handler.end(); it++) {
				BMessenger messenger(*it);
				messenger.SendMessage(new BMessage(MAPDATA_UPDATE));		
			}
		}
	}
};

static BUrlRequest*			request		= NULL;
static MapsData_Listener*	listener	= NULL;
static thread_id 			thread;

MapsData::MapsData() {

}

void MapsData::SetLongitude(float lon) {
	if (lon < -180.0) {
		longitude = 360.0  + lon;
	}
	else if (lon > 180.0) {
		longitude = -360.0  + lon;
	}
	else {
		longitude = lon;
	}
}

void MapsData::SetLatitude(float lat) {
	if (lat < -90.0) {
		latitude = -90.0;
	}
	else if (lat > 90.0) {
		latitude = 90.0;
	}
	else {
		latitude = lat;
	}
}

void MapsData::SetZoom(float _zoom) {
	zoom = _zoom;
	scale = 1.5 * pow(2.0, zoom);
}

void MapsData::AddHandler(BHandler* handle) {
	handler.push_back(handle);
}

void MapsData::Retrieve() {
	if (request != NULL)
	{
		request->Stop();
		wait_for_thread(thread, NULL);
		
		delete request;
		delete listener;
		
		request		= NULL;		
		listener	= NULL;
	}
	BString dataUrl;
	dataUrl.SetToFormat(baseUrl.String(), longitude, latitude, zoom, bearing, pitch, width, height);
	
	listener = new MapsData_Listener();
	request = BUrlProtocolRoster::MakeRequest(BUrl(dataUrl.String()), listener);

	thread = request->Run();
}

BMallocIO* MapsData::Get() {
	return data;
}

Vector2 MapsData::GetCoords() {
	Vector2 coords;
	
	coords.lon = longitude;
	coords.lat = latitude;
	
	return coords;
}	

float MapsData::GetScale() {
	return scale;
}
