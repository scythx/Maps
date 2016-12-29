#include "MapsView.h"

#include <stdio.h>

#include <Messenger.h>
#include <TranslationUtils.h>

#include <ScrollView.h>
#include <ListView.h>
#include <LayoutBuilder.h>
#include "MapsData.h"

MapsView::MapsView()
		: BView("mapsView", B_WILL_DRAW) {
	bitmap		= NULL;				

	virtualScroller = new VirtualScroller(this);

	MapsData::AddHandler(this);	
	MapsData::Retrieve();
}

MapsView::~MapsView() {

}

void MapsView::AddHandler(BHandler* handle) {
	handler.push_back(handle);
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
		
		for (std::vector<BHandler*>::iterator it = handler.begin(); it != handler.end(); it++) {
			BMessenger messenger(*it);
			messenger.SendMessage(new BMessage(M_MAPSVIEW_ON_FOCUS));		
		}
	}	
}

void MapsView::MouseUp(BPoint where) {
	if (IsMouseDown) {
		IsMouseDown = false;

		MapsVector mapsVector = MapsData::GetVector();
		if (pastPoint.x < where.x) {
			MapsData::SetLongitude(mapsVector.longitude - ((where.x - pastPoint.x) / mapsVector.scale));
		}
		else if (pastPoint.x > where.x) {
			MapsData::SetLongitude(mapsVector.longitude + ((pastPoint.x - where.x) / mapsVector.scale));
		}
		if (pastPoint.y < where.y) {
			MapsData::SetLatitude(mapsVector.latitude + ((where.y - pastPoint.y) / mapsVector.scale));
		}
		else if (pastPoint.y > where.y) {
			MapsData::SetLatitude(mapsVector.latitude - ((pastPoint.y - where.y) / mapsVector.scale));
		}
		MapsData::Retrieve();
	}
}

void MapsView::MessageReceived(BMessage* message) {
	switch (message->what) {
		case MAPDATA_UPDATE: {
			bitmap = BTranslationUtils::GetBitmap(MapsData::Get());
			Invalidate();
		}break;
		case VIRTUAL_SCROLLER: {
			MapsData::SetZoom(20 - (message->GetFloat("value", 0.0) / 3));
			MapsData::Retrieve();
		}break;
		default: {
			BView::MessageReceived(message);
		}break;
	}
}
