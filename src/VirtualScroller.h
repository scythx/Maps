#ifndef VIRTUALSCROLLER_H
#define VIRTUALSCROLLER_H

#include <ScrollBar.h>

#include "MapsView.h"

class VirtualScroller : public BScrollBar {
private:
	MapsView *view;
public:
	VirtualScroller(MapsView *target);
	virtual ~VirtualScroller();
	virtual void ValueChanged(float newValue);
};

#endif // VIRTUALSCROLLER_H
