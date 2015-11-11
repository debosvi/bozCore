
#ifndef _BOZ_SHADOW_INTERNAL_H_
#define _BOZ_SHADOW_INTERNAL_H_

#include "ShadowPublic.hpp"

namespace Bozo {
namespace Shadow {

class ShadowInternal {
public:
    ShadowInternal(ShadowPublic *parent, const std::string name, const int val);
    ~ShadowInternal();

private:
    ShadowPublic* _parent;
    std::string _name;
    int _val;
};

} // namespace Shadow
} // namespace Bozo

#endif // _BOZ_SHADOW_INTERNAL_H_
