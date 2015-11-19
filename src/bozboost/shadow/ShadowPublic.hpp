
#ifndef _BOZ_SHADOW_H_
#define _BOZ_SHADOW_H_

#include <string>
#include <boost/signals2.hpp>
#include <boost/shared_ptr.hpp>

namespace Bozo {
namespace Shadow {

class ShadowInternal;

typedef boost::signals2::signal<void (const std::string name, const int val)> onEvent;
typedef onEvent::slot_type onEventSlotType;

class ShadowPublic {
    friend class ShadowInternal;
    
public:
    ShadowPublic(const std::string name, const int val);
    ~ShadowPublic();
    boost::signals2::connection doOnEvent(const onEventSlotType & slot);
    
private:
    boost::shared_ptr<ShadowInternal> _internal;
    onEvent _onEventSig;
};

} // namespace Shadow
} // namespace Bozo

#endif // _BOZ_SHADOW_H_
