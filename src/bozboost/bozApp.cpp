
#include "bozApp.hpp"

bozApp::bozApp(const int val) : _val(val) {
	std::cout << __PRETTY_FUNCTION__ << ", val: " << val << std::endl;
	
}

bozApp::~bozApp() {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void bozApp::process(void) {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	sleep(_val);
	std::cout << __PRETTY_FUNCTION__ << "emit" << std::endl;
	
	this->_onChange(_val);
}