#include "MapsData.h"

#include <Messenger.h>
#include <UrlProtocolRoster.h>
#include <UrlProtocolListener.h>

class MapsData_Listener : public BUrlProtocolListener, protected MapsData {
public:
	MapsData_Listener() {
		if (MapsData::data != NULL) {
			delete MapsData::data;
			MapsData::data = NULL;
		}
		MapsData::data = new BMallocIO();
	}
	virtual void DataReceived(BUrlRequest* caller, const char* _data, off_t position, ssize_t size) {
		MapsData::data->WriteAt(position, _data, size);
	}
	virtual void RequestCompleted(BUrlRequest* caller, bool success) {
		if (success) {
			for (std::vector<BHandler*>::iterator it = MapsData::handler.begin(); it != MapsData::handler.end(); it++) {
				BMessenger messenger(*it);
				messenger.SendMessage(new BMessage(MAPDATA_UPDATE));		
			}
		}
	}
};

// Default scale between BPoint and the maps tile
// Change this value if you have more accurate value
#define DEFAULT_BPOINT_SCALE 1.5f

BMallocIO* 				MapsData::data = NULL;
std::vector<BHandler*>	MapsData::handler;

MapsVector 			MapsData::mapsVector;

thread_id			MapsData::thread;
BUrlRequest*		MapsData::request	= NULL;
MapsData_Listener*	MapsData::listener	= NULL;

BString MapsData::baseUrl("https://api.mapbox.com/styles/v1/mapbox/streets-v8/static/%f,%f,%f,%f,%f/%dx%d?access_token=pk.eyJ1IjoicmFlZmFsZGhpYSIsImEiOiJjaXdnN3J0YTkwMTV1MnVraXgzNGowbTBuIn0.9RYCJF1sfuUD86QRuBItYw&attribution=false&logo=false");

MapsData::MapsData() {

}

MapsData::~MapsData() {

}

void MapsData::Initialize(int width, int height) {
	mapsVector.width	 = width;
	mapsVector.height	 = height;

	mapsVector.scale	 = DEFAULT_BPOINT_SCALE;

	mapsVector.longitude = 0.0f;
	mapsVector.latitude  = 0.0f;
	mapsVector.zoom 	 = 0.0f;
	mapsVector.bearing	 = 0.0f;
	mapsVector.pitch	 = 0.0f;
}

void MapsData::SetLongitude(float longitude) {
	if (longitude < -180.0) {
		mapsVector.longitude = 360.0 + longitude;
	}
	else if (longitude > 180.0) {
		mapsVector.longitude = -360.0  + longitude;
	}
	else {
		mapsVector.longitude = longitude;
	}
}

void MapsData::SetLatitude(float latitude) {
	if (latitude < -90.0) {
		mapsVector.latitude = -90.0;
	}
	else if (latitude > 90.0) {
		mapsVector.latitude = 90.0;
	}
	else {
		mapsVector.latitude = latitude;
	}
}

void MapsData::SetZoom(float zoom) {
	mapsVector.zoom = zoom;
	mapsVector.scale = DEFAULT_BPOINT_SCALE * pow(2.0, mapsVector.zoom);
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
	dataUrl.SetToFormat(baseUrl.String(), mapsVector.longitude, mapsVector.latitude, mapsVector.zoom, mapsVector.bearing, mapsVector.pitch, mapsVector.width, mapsVector.height);
	
	listener = new MapsData_Listener();
	request = BUrlProtocolRoster::MakeRequest(BUrl(dataUrl.String()), listener);

	thread = request->Run();
}

BMallocIO* MapsData::Get() {
	return data;
}

MapsVector MapsData::GetVector() {
	return mapsVector;
}	
