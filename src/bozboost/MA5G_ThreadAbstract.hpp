
#ifndef _MA5G_THREAD_ABSTRACT_H_
#define _MA5G_THREAD_ABSTRACT_H_

#include <boost/thread.hpp>

namespace Morpho {
namespace MA5G {
namespace Thread {

typedef boost::function<int (void*)> processFunc;

class MA5G_ThreadAbstract {

public:
	// constructors and destructors
    explicit MA5G_ThreadAbstract(const int type = LongTerm);
	~MA5G_ThreadAbstract();
	
	// public APIs
	int setProcess(processFunc f);
	int start(void);
	int stop(void);

	// constants and definitions
    static int OneShot;
    static int LongTerm;
	
private:
    // private members
    processFunc _func;
    boost::mutex _mut;
	boost::thread* _th;
	int _type;

    // private APIs
    int defaultFunc(void*);
    virtual int internalProcess(void) = 0;
    
};

} // namespace Thread;
} // namespace MA5G;
} // namespace Morpho;

#endif // _MA5G_THREAD_ABSTRACT_H_
