
#ifndef _BOZ_SHADOW_H_
#define _BOZ_SHADOW_H_

#include <string>
#include <boost/shared_ptr.hpp>

namespace Bozo {
namespace Shadow {

class ShadowInternal;

class ShadowPublic {
public:
    ShadowPublic(const std::string name, const int val);
    ~ShadowPublic();

private:
    boost::shared_ptr<ShadowInternal> _internal;
};

} // namespace Shadow
} // namespace Bozo

#endif // _BOZ_SHADOW_H_
