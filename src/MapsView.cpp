#include "MapsView.h"

#include <stdio.h>

#include <Messenger.h>
#include <TranslationUtils.h>

#include "MapsData.h"

MapsView::MapsView(float _longitude, float _latitude, float _zoom, 
		float _bearing, float _pitch, int _width, int _height)
		: BView("mapsView", B_WILL_DRAW) {
	bitmap		= NULL;				

	virtualScroller = new VirtualScroller(this);

	MapsData::SetLongitude(_longitude);
	MapsData::SetLatitude(_latitude);
	
	MapsData::Retrieve();
}

MapsView::~MapsView() {

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

		Vector2 coords = MapsData::GetCoords();
		
		if (pastPoint.x < where.x) {
			MapsData::SetLongitude(coords.lon - ((where.x - pastPoint.x) / MapsData::GetScale()));
		}
		else if (pastPoint.x > where.x) {
			MapsData::SetLongitude(coords.lon + ((pastPoint.x - where.x) / MapsData::GetScale()));
		}
		if (pastPoint.y < where.y) {
			MapsData::SetLatitude(coords.lat + ((where.y - pastPoint.y) / MapsData::GetScale()));
		}
		else if (pastPoint.y > where.y) {
			MapsData::SetLatitude(coords.lat - ((pastPoint.y - where.y) / MapsData::GetScale()));
		}
		MapsData::Retrieve();
	}
}

void MapsView::MessageReceived(BMessage* message) {
	switch (message->what) {
		case MAPDATA_UPDATE: {
			bitmap = BTranslationUtils::GetBitmap(MapsData::Get());
			Invalidate();
			
			break;		
		}
		case VIRTUAL_SCROLLER: {
			MapsData::SetZoom(20 - (message->GetFloat("value", 0.0) / 3));
			MapsData::Retrieve();
			
			break;
		}
		default: {
			BView::MessageReceived(message);
			break;
		}
	}
}
