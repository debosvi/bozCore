
#ifndef _MA5G_THREAD_ABSTRACT_H_
#define _MA5G_THREAD_ABSTRACT_H_

#include <boost/thread.hpp>

namespace Morpho {
namespace MA5G {
namespace Thread {
	
typedef enum {
	PRIORITY_VERY_HIGH = 0,
	PRIORITY_HIGH,
	PRIORITY_NORMAL,
	PRIORITY_LOW,
	PRIORITY_COUNT
} MA5G_ThreadPriority;

class MA5G_ThreadAbstract {

public:
	// constructors and destructors
    explicit MA5G_ThreadAbstract(const int type = LongTerm);
	~MA5G_ThreadAbstract();
	
	// public APIs
	int start(void);
	int stop(void);
	int setSchedPriority(const MA5G_ThreadPriority prio);
	int setName(const char* const name);
	
	// constants and definitions
    static int OneShot;
    static int LongTerm;
	
private:
    // private members
    boost::mutex _mut;
	boost::thread* _th;
	int _type;

    // private APIs
    int defaultFunc(void*);
    virtual int internalProcess(void);
    virtual int loopProcess(void) = 0;
    
};

} // namespace Thread;
} // namespace MA5G;
} // namespace Morpho;

#endif // _MA5G_THREAD_ABSTRACT_H_
