
#include "MyAppClass.hpp"

namespace Morpho {
namespace MA5G {

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

} // namespace MA5G;
} // namespace Morpho;

