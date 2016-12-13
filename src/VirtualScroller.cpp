#include "VirtualScroller.h"

VirtualScroller::VirtualScroller(MapsView *target) : BScrollBar(NULL, NULL, 0, 60, B_VERTICAL) {
	SetTarget(target);
	view = target;
	SetValue(60);
}

VirtualScroller::~VirtualScroller() {
	
}

void VirtualScroller::ValueChanged(float newValue) {
	view->SetZoom(20 - (newValue / 3));
	view->Refresh();
}
