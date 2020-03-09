#include "BreakoutApp.h"

int main() {
	
	// allocation
	auto app = new BreakoutApp();

	// initialise and loop
	app->run("AIE", 1920, 1080, true);

	// deallocation
	delete app;

	return 0;
}