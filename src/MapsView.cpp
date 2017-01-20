/*
 * Copyright 2016-2017 Raefaldhi Amartya Junior <raefaldhiamartya@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

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
	IsTransformNext = false;

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
		if (IsTransformNext) {
			translatePoint.x += (where.x - pastPoint.x);
			translatePoint.y += (where.y - pastPoint.y);
		}
		else {
			translatePoint.x = (where.x - pastPoint.x);
			translatePoint.y = (where.y - pastPoint.y);
		}
		IsTransformNext = true;

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

void MapsView::MouseMoved(BPoint where, uint32 code, const BMessage* dragMessage) {
	if (IsMouseDown) {
		MapsVector mapsVector = MapsData::GetVector();

		BAffineTransform transform;
		if (IsTransformNext) {
			transform.PreTranslateBy(translatePoint.x, translatePoint.y);
		}
		transform.TranslateBy((where.x - pastPoint.x), (where.y - pastPoint.y));
		SetTransform(transform);
	}
}

void MapsView::MessageReceived(BMessage* message) {
	switch (message->what) {
		case MAPDATA_UPDATE: {
			bitmap = BTranslationUtils::GetBitmap(MapsData::Get());
			IsTransformNext = false;
			translatePoint.x = 0;
			translatePoint.y = 0;

			BAffineTransform transform;
			SetTransform(transform);
			Invalidate();
		}break;
		case VIRTUAL_SCROLLER: {
			MapsVector mapsVector0 = MapsData::GetVector();
			MapsData::SetZoom(20 - (message->GetFloat("value", 0.0) / 3));
			MapsVector mapsVector1 = MapsData::GetVector();

			BPoint center;
			BPoint where;
			
			center.x = Bounds().right / 2;
			center.y = Bounds().bottom / 2;
			GetMouse(&where, NULL);
			
			if (center.x != where.x) {
				if (center.x < where.x) {
					MapsData::SetLongitude(mapsVector1.longitude + ((where.x - center.x) / mapsVector1.scale));
				}
				else {
					MapsData::SetLongitude(mapsVector1.longitude - ((center.x - where.x) / mapsVector1.scale));
				}
			}
			if (center.y != where.y) {
				if (center.y < where.y) {
					MapsData::SetLatitude(mapsVector1.latitude - ((where.y - center.y) / mapsVector1.scale));
				}
				else {
					MapsData::SetLatitude(mapsVector1.latitude + ((center.y - where.y) / mapsVector1.scale));
				}
			}
			
			mapsVector1 = MapsData::GetVector();
			BAffineTransform transform;
			if (mapsVector1.zoom > mapsVector0.zoom) {
				transform.ScaleBy(where, pow(2.0, mapsVector1.zoom));
			}
			else if (mapsVector1.zoom != 0) {
				transform.ScaleBy(where, sqrt(mapsVector1.zoom));
			}
			SetTransform(transform);
			MapsData::Retrieve();
		}break;
		default: {
			BView::MessageReceived(message);
		}break;
	}
}
