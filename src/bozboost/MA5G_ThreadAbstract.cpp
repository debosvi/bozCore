
#include "MA5G_ThreadAbstract.hpp"

namespace Morpho {
namespace MA5G {
namespace Thread {

// constants and definitions
int MA5G_ThreadAbstract::OneShot=0;
int MA5G_ThreadAbstract::LongTerm=1;

MA5G_ThreadAbstract::MA5G_ThreadAbstract(const int type) : _th(NULL), _type(type) {
	std::cerr << __PRETTY_FUNCTION__ << std::endl;    
}

MA5G_ThreadAbstract::~MA5G_ThreadAbstract() {
	std::cerr << __PRETTY_FUNCTION__ << std::endl;
    if(_th) {
		stop();
		delete _th;
	}
}

int MA5G_ThreadAbstract::start(void) {
	std::cerr << __PRETTY_FUNCTION__ << std::endl;
	_th = new boost::thread(&MA5G_ThreadAbstract::internalProcess, this);
    return 0;
}

int MA5G_ThreadAbstract::stop(void) {
	std::cerr << __PRETTY_FUNCTION__ << std::endl;
	_th->join();
    return 0;
}

int MA5G_ThreadAbstract::internalProcess(void) {
	std::cerr << __PRETTY_FUNCTION__ << std::endl;
	while (1) {
		if(loopProcess() < 0)
			break;
		
		if(_type==OneShot)
			break;
		
		// any mechanism avoiding CPU load (active detection possible)
		sleep(1);
	}
	
	return 0;
}

} // namespace Thread;
} // namespace MA5G;
} // namespace Morpho
