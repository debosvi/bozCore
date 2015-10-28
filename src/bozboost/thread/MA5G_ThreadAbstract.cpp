/*!
 * \file MA5G_ThreadAbstract.cpp
 * \brief Abstract template encapsulation class implementation.
 * \author Vincent de RIBOU
 * \version 0.1
 */
 
#define _USE_GNU

#include <pthread.h>
#include <errno.h>

#include "MA5G_ThreadAbstract.hpp"

namespace Morpho {
namespace MA5G {
namespace Thread {
	
/**
* \brief enumeration for Thread Priority
*/
enum Thread_priorites {
	very_high_thread_priority = 40,
	high_thread_priority = 35,
	normal_thread_priority = 20,
	low_thread_priority = 1
};

MA5G_ThreadAbstract::MA5G_ThreadAbstract(const std::string name, const MA5G_ThreadType type) : 
	_th(NULL), _name(name), _prio(), _type(type), _stop(0), _update(0) {
	
	std::cerr << __PRETTY_FUNCTION__ << std::endl;    
	prio_values.insert(std::make_pair(PRIORITY_LOW, low_thread_priority));
	prio_values.insert(std::make_pair(PRIORITY_NORMAL, normal_thread_priority));
	prio_values.insert(std::make_pair(PRIORITY_HIGH, high_thread_priority));
	prio_values.insert(std::make_pair(PRIORITY_VERY_HIGH, very_high_thread_priority));
}

MA5G_ThreadAbstract::~MA5G_ThreadAbstract() {
	std::cerr << __PRETTY_FUNCTION__ << std::endl;
    if(_th) {
		stop();
		delete _th;
	}
}

int MA5G_ThreadAbstract::setSchedPriority(const MA5G_ThreadPriority prio) {
	std::cerr << __PRETTY_FUNCTION__ << std::endl;
	
	boost::lock_guard<boost::mutex> guard(_mut);	
	_prio=prio;
	_update=1;
	return 0;
}

int MA5G_ThreadAbstract::setName(const std::string name) {
	std::cerr << __PRETTY_FUNCTION__ << std::endl;
	
	boost::lock_guard<boost::mutex> guard(_mut);	
	_name=name;
	// pthread limit
	_name.resize(15);
	_update=1;
	return 0;
}


int MA5G_ThreadAbstract::start(void) {
	std::cerr << __PRETTY_FUNCTION__ << std::endl;
	_th = new boost::thread(&MA5G_ThreadAbstract::internalProcess, this);
	
	if(!_th) 
		return (errno=EFAULT,-1);
	
    return (errno=0,0);
}

int MA5G_ThreadAbstract::stop(void) {
	std::cerr << __PRETTY_FUNCTION__ << std::endl;
	
	boost::lock_guard<boost::mutex> guard(_mut);	
	_stop=1;
	if(_th) 
		_th->join();
    return 0;
}

int MA5G_ThreadAbstract::internalProcess(void) {
	std::cerr << __PRETTY_FUNCTION__ << std::endl;
	while (1) {
		{
			boost::lock_guard<boost::mutex> guard(_mut);	
			if(_update) {
				if(pthread_setname_np(_th->native_handle(), _name.c_str())<0)
					std::cerr << __PRETTY_FUNCTION__ << "pthread_setname_np error (" << errno << "/" << strerror(errno) << ")" << std::endl;
				
				if(nice(prio_values.at(_prio))<0)
					std::cerr << __PRETTY_FUNCTION__ << "pthread_setname_np error (" << errno <<"/" << strerror(errno) << ")" << std::endl;
				
				_update=0;
			}
			
		}

		if(loopProcess() < 0) {
			break;
		}
		
		if((_type==TYPE_ONESHOT) || _stop )
			break;
		
		// any mechanism avoiding CPU load (active detection possible)
		sleep(1);
	}
	
	return 0;
}

} // namespace Thread;
} // namespace MA5G;
} // namespace Morpho
