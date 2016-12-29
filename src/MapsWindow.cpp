#include "MapsWindow.h"

#include <LayoutBuilder.h>
#include <ListView.h>
#include <ScrollView.h>

#include "MapsData.h"
#include "MapsView.h"
#include "SearchForm.h"

MapsWindow::MapsWindow() : BWindow(BRect(100, 100, 500, 500), "Maps",
		B_TITLED_WINDOW, B_NOT_RESIZABLE | B_QUIT_ON_WINDOW_CLOSE) {
// 	Initialize MapsData otherwise it only shows 0x0 tile, and might
//	causes crash because scale isn't initialized to 1.5.
	MapsData::Initialize(400, 400);
	
	SearchForm* searchForm = new SearchForm();	

	MapsView* mapsView = new MapsView();
	mapsView->AddHandler(searchForm->GetSearchBar());
	mapsView->AddHandler(searchForm->GetSearchResultList());

	BScrollView* resultListScroller = new BScrollView("resultListScroller", searchForm->GetSearchResultList(), 0, false, true);
	BLayoutBuilder::Group<>(mapsView, B_VERTICAL, 0)
		.SetInsets(5)
		.AddGrid(0.0f, 0.0f)
			.Add(resultListScroller, 0, 0)
		.End()
		.AddGlue(3)
	.End();
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.SetInsets(0)
		.AddGrid(0.0f, 0.0f)
			.Add(searchForm->GetSearchBar(), 0, 0)
		.End()
		.Add(mapsView)
	.End();
}

MapsWindow::~MapsWindow() {

}
