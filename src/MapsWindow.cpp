#include "MapsWindow.h"

#include <ScrollBar.h>
#include <View.h>
#include <Window.h>

#include "MapsView.h"
//#include "VirtualScroller.h"

MapsWindow::MapsWindow() : BWindow(BRect(100, 100, 500, 500), "Maps",
					B_TITLED_WINDOW, B_NOT_RESIZABLE | B_QUIT_ON_WINDOW_CLOSE) {		
	mapsView = new MapsView();
	mapsView->Invalidate();

	AddChild(mapsView);
}

MapsWindow::~MapsWindow() {
	
}
