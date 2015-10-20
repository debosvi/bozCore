
#include "bozApp.hpp"

bozApp::bozApp(const int val) : _val(val) {
	std::cerr << __PRETTY_FUNCTION__ << ", val: " << _val << std::endl;
	
}

bozApp::~bozApp() {
	std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void bozApp::process(void) {
	std::cerr << __PRETTY_FUNCTION__ << ", val: " << _val << std::endl;
	sleep(_val);
	std::cerr << __PRETTY_FUNCTION__ << ", emit" << std::endl;
	
	this->_onChange(_val);
}