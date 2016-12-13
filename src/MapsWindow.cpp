#include "MapsWindow.h"

#include <LayoutBuilder.h>
#include <ScrollBar.h>
#include <View.h>
#include <Window.h>

#include "MapsView.h"
#include "VirtualScroller.h"

MapsWindow::MapsWindow() : BWindow(BRect(100, 100, 500, 500), "Maps",
					B_TITLED_WINDOW, B_NOT_RESIZABLE | B_QUIT_ON_WINDOW_CLOSE) {		
	mapsView = new MapsView(120, 0, 2, 0, 0, 400, 400);
	VirtualScroller* virtualScroller = new VirtualScroller(mapsView);

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.SetInsets(0)
		.Add(mapsView)
		.End();
}

MapsWindow::~MapsWindow() {
	
}
