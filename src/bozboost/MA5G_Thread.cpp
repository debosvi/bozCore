
#include "MA5G_Thread.hpp"

namespace Morpho {
namespace MA5G {
namespace Thread {

MA5G_Thread::MA5G_Thread(const int type) : MA5G_ThreadAbstract(type) {
	std::cerr << __PRETTY_FUNCTION__ << std::endl;	
}

MA5G_Thread::~MA5G_Thread() {
	std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

} // namespace Thread;
} // namespace MA5G;
} // namespace Morpho
