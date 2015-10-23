
#ifndef _MYAPP_CLASS_H_
#define _MYAPP_CLASS_H_

#include "MyAppThread.hpp"

namespace Morpho {
namespace MA5G {

class MyAppClass  {
	// ici on accède aux membres privés de la classe mère
	friend class Morpho::MA5G::MyAppThread;
	
public:
	explicit MyAppClass(void);
	~MyAppClass();
	
	void doIt(void);
	
private:
	int _val;	
	Morpho::MA5G::MyAppThread _th;
};

} // namespace MA5G;
} // namespace Morpho;

#endif // _MYAPP_CLASS_H_
