/*
 * Copyright 2016-2017 Raefaldhi Amartya Junior <raefaldhiamartya@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "VirtualScroller.h"

#include <Messenger.h>

VirtualScroller::VirtualScroller(BView* target)
		: BScrollBar(NULL, target, 0, 60, B_VERTICAL) {
	SetValue(60);
}

VirtualScroller::~VirtualScroller() {
	
}

void VirtualScroller::ValueChanged(float newValue) {
	static bool once = false;
	if (once) {
		BMessenger messenger(Target());
		BMessage* message = new BMessage(VIRTUAL_SCROLLER);
	
		message->AddFloat("value", newValue);
		messenger.SendMessage(message);	
		
		return;
	}
	once = true;
}
