
#include <iostream>

#include "ShadowInternal.hpp"

namespace Bozo {
namespace Shadow {

ShadowInternal::ShadowInternal(ShadowPublic *parent, const std::string name, const int val) : 
    _parent(parent), _name(name), _val(val) {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

ShadowInternal::~ShadowInternal() {
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

} // namespace Shadow
} // namespace Bozo
