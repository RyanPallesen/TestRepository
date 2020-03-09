#include "BreakoutGameApp.h"

int main() {
	
	// allocation
	auto app = new BreakoutGameApp();

	// initialise and loop
	app->run("AIE", 1920, 1080, false);

	// deallocation
	delete app;

	return 0;
}