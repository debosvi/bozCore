
#include "StateMachine.hpp"
#include "priv/StateMachinePrivate.hpp"

namespace boz {
namespace sm {
    
StateMachine::StateMachine(State *parent) : 
    State(parent), _members(new StateMachinePrivate()) {
        
}

StateMachine::~StateMachine() {
    delete _members;
}    
    
void StateMachine::onEntry() {
    
}
    
void StateMachine::onExit() {
    
}

void StateMachine::addState(BaseState* state) {
    
}

void StateMachine::removeState(BaseState* state) {
    
}

void StateMachine::start() {
    
}

void StateMachine::stop() {
    
}

} // namespace sm
} // namespace boz