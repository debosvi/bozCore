
#include <iostream>

#include "ShadowPublic.hpp"
#include "ShadowInternal.hpp"

namespace Bozo {
namespace Shadow {

ShadowPublic::ShadowPublic(const std::string name, const int val) : 
    _internal(new ShadowInternal(this, name, val)) {
    
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

ShadowPublic::~ShadowPublic() {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

boost::signals2::connection ShadowPublic::doOnEvent(const onEventSlotType & slot) {
    return _onEventSig.connect(slot);
}

} // namespace Shadow
} // namespace Bozo
