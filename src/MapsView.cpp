#include "MapsView.h"

MapsView::MapsView() : BView(BRect(0, 0, 400, 400), NULL, B_FOLLOW_ALL, B_WILL_DRAW) {
	tile = new MapsTile();
	AddChild(tile);
}

MapsView::~MapsView() {

}

MapsTile::MapsTile() : BView("Tile", B_WILL_DRAW) {
	ResizeBy(50, 50);

	SetViewColor(180,180,0);
}

MapsTile::~MapsTile() {

}

void MapsTile::MouseDown(BPoint where) {
	syncval = where;
	panning = true;
}

void MapsTile::MouseUp(BPoint where) {	
	panning = false;
}

void MapsTile::MouseMoved(BPoint where, uint32 code, const BMessage* dragMessage) {	
	if (panning) {
		BPoint dest = ConvertToParent(where);
		
		MoveTo(dest.x - syncval.x, dest.y - syncval.y);
		Invalidate();
	}
}
