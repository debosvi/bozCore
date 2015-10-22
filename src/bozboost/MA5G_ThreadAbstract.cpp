
#include "MA5G_ThreadAbstract.hpp"

namespace Morpho {
namespace MA5G {
namespace Thread {

// constants and definitions
int OneShot=0;
int LongTerm=1;

MA5G_ThreadAbstract::MA5G_ThreadAbstract(const int type) : _th(NULL), _type(type) {
	std::cerr << __PRETTY_FUNCTION__ << std::endl;
    _func = boost::bind(&MA5G_ThreadAbstract::defaultFunc, this, _1);
}

MA5G_ThreadAbstract::~MA5G_ThreadAbstract() {
	std::cerr << __PRETTY_FUNCTION__ << std::endl;
    if(_th) delete _th;
}

int MA5G_ThreadAbstract::defaultFunc(void *arg) {
	std::cerr << __PRETTY_FUNCTION__ << std::endl;
	(void)arg;
    return 0;
}

int MA5G_ThreadAbstract::setProcess(processFunc f) {
	std::cerr << __PRETTY_FUNCTION__ << std::endl;
	boost::lock_guard<boost::mutex> lock(_mut);
    _func = f;
    return 0;
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

} // namespace Thread;
} // namespace MA5G;
} // namespace Morpho
