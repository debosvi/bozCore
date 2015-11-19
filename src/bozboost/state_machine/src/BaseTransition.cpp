
#include "BaseTransition.hpp"
#include "State.hpp"

namespace boz {
namespace sm {
    
BaseTransition::BaseTransition(State *source) : 
    _source(source), _target(0) {
        
}

BaseTransition::~BaseTransition() {

}    
    
StateMachine* BaseTransition::machine() const { 
    if(_source) 
        return _source->machine();
    return 0;
}

State* BaseTransition::sourceState() const { return _source; }
BaseState* BaseTransition::targetState() const { return _target; }

void BaseTransition::setTargetState(BaseState* target) {
    _target = target;
}

} // namespace sm
} // namespace boz