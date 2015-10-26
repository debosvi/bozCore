/*!
 * \file MyAppClass.cpp
 * \brief Class embedding thread specific class implementation.
 * \author Vincent de RIBOU
 * \version 0.1
 */
 
#include "MyAppClass.hpp"

namespace Morpho {
namespace MA5G {
namespace Example {

MyAppClass::MyAppClass(void) : _val(0), _th(this) {
	std::cerr << __PRETTY_FUNCTION__ << std::endl;    
	
}
	
MyAppClass::~MyAppClass() {
	std::cerr << __PRETTY_FUNCTION__ << std::endl;    
	
}
	
void MyAppClass::doIt(void) {
	std::cerr << __PRETTY_FUNCTION__ << std::endl;    
	_th.start();
}

} // namespace Example;
} // namespace MA5G;
} // namespace Morpho;

