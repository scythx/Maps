#ifndef VIRTUALSCROLLER_H
#define VIRTUALSCROLLER_H

#include <ScrollBar.h>
#include <View.h>

enum {
	VIRTUAL_SCROLLER = 0x0002
};

class VirtualScroller : public BScrollBar {
private:
	BView *view;
public:
	VirtualScroller(BView *target);
	virtual ~VirtualScroller();
	virtual void ValueChanged(float newValue);
};

#endif // VIRTUALSCROLLER_H
