
#include <iostream>

#include "ShadowInternal.hpp"

namespace Bozo {
namespace Shadow {

ShadowInternal::ShadowInternal(ShadowPublic *parent, const std::string name, const int val) : 
    _parent(parent), _name(name), _val(val) {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
    boost::shared_ptr<boost::thread> th(new boost::thread(boost::bind(&ShadowInternal::process, this)));
    _thread = th;
}

ShadowInternal::~ShadowInternal() {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

void ShadowInternal::process(void) {
    for(int i = 0; i<5; i++) {
        std::cerr << __PRETTY_FUNCTION__ << std::endl;
        _parent->_onEventSig(_name, _val);
        boost::this_thread::sleep(boost::posix_time::seconds(1));
    }
}

} // namespace Shadow
} // namespace Bozo
