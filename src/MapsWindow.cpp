#include "MapsWindow.h"

#include <LayoutBuilder.h>

#include "MapsData.h"
#include "MapsView.h"

MapsWindow::MapsWindow() : BWindow(BRect(100, 100, 500, 500), "Maps",
		B_TITLED_WINDOW, B_NOT_RESIZABLE | B_QUIT_ON_WINDOW_CLOSE) {
	MapsView* mapsView = new MapsView(120, 0, 2, 0, 0, 400, 400);
	MapsData::AddHandler(mapsView);
	
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.SetInsets(0)
		.Add(mapsView)
	.End();
}

MapsWindow::~MapsWindow() {

}
