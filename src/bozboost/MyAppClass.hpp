/*!
 * \file MyAppClass.hpp
 * \brief Class embedding thread specific class interface.
 * \author Vincent de RIBOU
 * \version 0.1
 */
 
#ifndef _MYAPP_CLASS_H_
#define _MYAPP_CLASS_H_

#include "MyAppThread.hpp"

namespace Morpho {
namespace MA5G {
namespace Example {

class MyAppClass  {
	// ici on accède aux membres privés de la classe mère
	friend class Morpho::MA5G::Example::MyAppThread;
	
public:
	explicit MyAppClass(void);
	~MyAppClass();
	
	void doIt(void);
	
private:
	int _val;	
	Morpho::MA5G::Example::MyAppThread _th;
};

} // namespace Example;
} // namespace MA5G;
} // namespace Morpho;

#endif // _MYAPP_CLASS_H_
