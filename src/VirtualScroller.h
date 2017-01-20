/*
 * Copyright 2016-2017 Raefaldhi Amartya Junior <raefaldhiamartya@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#ifndef VIRTUALSCROLLER_H
#define VIRTUALSCROLLER_H

#include <ScrollBar.h>
#include <View.h>

enum {
	VIRTUAL_SCROLLER = 0x0002
};

class VirtualScroller : public BScrollBar {
public:
	VirtualScroller(BView *target);
	virtual ~VirtualScroller();
	virtual void ValueChanged(float newValue);
};

#endif // VIRTUALSCROLLER_H
