
#ifndef _MYAPP_THREAD_H_
#define _MYAPP_THREAD_H_

#include "MA5G_ThreadAbstract.hpp"

namespace Morpho {
namespace MA5G {

class MyAppClass;

class MyAppThread : public Morpho::MA5G::Thread::MA5G_ThreadAbstract {
	
public:
	MyAppThread(MyAppClass* parent);
	~MyAppThread();
	
private:
	virtual int loopProcess(void);
	MyAppClass* _parent;

};

} // namespace MA5G;
} // namespace Morpho;

#endif // _MYAPP_THREAD_H_
