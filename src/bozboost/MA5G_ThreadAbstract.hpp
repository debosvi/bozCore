
#ifndef _MA5G_THREAD_ABSTRACT_H_
#define _MA5G_THREAD_ABSTRACT_H_

#include <boost/thread.hpp>

namespace Morpho {
namespace MA5G {
namespace Thread {

class MA5G_ThreadAbstract {

public:
	// constructors and destructors
    explicit MA5G_ThreadAbstract(const int type = LongTerm);
	~MA5G_ThreadAbstract();
	
	// public APIs
	int start(void);
	int stop(void);

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
