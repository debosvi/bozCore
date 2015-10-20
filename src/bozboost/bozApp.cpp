
#include "bozApp.hpp"

bozApp::bozApp() {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	
}

bozApp::~bozApp() {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void bozApp::process(void) {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	sleep(5);
	std::cout << __PRETTY_FUNCTION__ << "emit" << std::endl;
	
	this->_onChange(3);
}