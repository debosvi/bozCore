
#include "State.hpp"

namespace boz {
namespace sm {
    
State::State(State *parent) : 
    BaseState(parent), _initState(0) {
        
}

State::~State() {

}    
    
void State::onEntry() {
    
}
    
void State::onExit() {
    
}

BaseState* State::initialState() const { return _initState; }

void State::setInitialState(BaseState* state) {
    _initState = state;
}

} // namespace sm
} // namespace boz