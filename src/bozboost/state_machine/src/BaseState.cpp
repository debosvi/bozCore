
#include "BaseState.hpp"

namespace boz {
namespace sm {
    
BaseState::BaseState(State *parent) : 
    _parent(parent), _active(false) {
        
}

BaseState::~BaseState() {

}    
    
bool BaseState::active() const { return _active; }
StateMachine* BaseState::machine() const { return _machine; }
State* BaseState::parentState() const { return _parent; }

} // namespace sm
} // namespace boz