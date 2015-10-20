
#include "bozApp.hpp"

static void onChangeSlot(int x) {
	std::cout << __PRETTY_FUNCTION__ << ", arg: " << x << std::endl;
}
int main(int ac, char **av) {
	bozApp app;
	app.doOnChange(onChangeSlot);
	
	app.start();
	// app.stop();
}