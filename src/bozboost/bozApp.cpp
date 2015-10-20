
#include "bozApp.hpp"

bozApp::bozApp() {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	
}

bozApp::~bozApp() {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	stop();
}

boost::signals2::connection bozApp::doOnChange(const OnChangeSlotType & slot) {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	return _onChange.connect(slot);
}


void bozApp::start(void) {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	_th = boost::thread(&bozApp::process, this);
}

void bozApp::stop(void) {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	_th.join();
}

void bozApp::process(void) {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	sleep(5);
	std::cout << __PRETTY_FUNCTION__ << "emit" << std::endl;
	
	this->_onChange(3);
}