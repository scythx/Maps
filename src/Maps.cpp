#include "Maps.h"

Maps::Maps() : BApplication("application/x-vnd.Haiku-Maps") {
	mapsWindow = new MapsWindow();
	mapsWindow->Show();
}

Maps::~Maps() {
	delete mapsWindow;
}

int main(int argc, char *argv[]) {
	Maps maps;
	maps.Run();
	
	return 0;
}
