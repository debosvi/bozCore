/*!
 * \file MyAppThread.cpp
 * \brief Class describing specific threaded worker implementation.
 * \author Vincent de RIBOU
 * \version 0.1
 */
 
#include "MyAppThread.hpp"
#include "MyAppClass.hpp"

using namespace Morpho::MA5G::Thread;

namespace Morpho {
namespace MA5G {
namespace Example {
		
MyAppThread::MyAppThread(MyAppClass* parent) : MA5G_ThreadAbstract("MyAppThread", TYPE_LONGTERM), 
	_parent(parent) {
	std::cerr << __PRETTY_FUNCTION__ << std::endl;	
	setSchedPriority(Thread::PRIORITY_HIGH);
	setName(__PRETTY_FUNCTION__);
}

MyAppThread::~MyAppThread() {
	std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

// ici on accède aux membres privés de la classe mère
int MyAppThread::loopProcess(void) {
	std::cerr << __PRETTY_FUNCTION__ << _parent->_val << std::endl;
		
	_parent->_val++;
	if(_parent->_val>10)
		return -1;
	
	return 0;
}

} // namespace Example;
} // namespace MA5G;
} // namespace Morpho
