
#include "bozAppAbstract.hpp"

bozAppAbstract::~bozAppAbstract() {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	stop();
}

boost::signals2::connection bozAppAbstract::doOnChange(const OnChangeSlotType & slot) {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	return _onChange.connect(slot);
}


void bozAppAbstract::start(void) {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	_th = boost::thread(&bozAppAbstract::process, this);
}

void bozAppAbstract::stop(void) {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	_th.join();
}
