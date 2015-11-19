
#include "StateMachine.hpp"

namespace boz {
namespace sm {
    
StateMachine::StateMachine(State *parent) : 
    State(parent) {
        
}

StateMachine::~StateMachine() {

}    
    
void StateMachine::onEntry() {
    
}
    
void StateMachine::onExit() {
    
}

void StateMachine::start() {
    
}

void StateMachine::stop() {
    
}

} // namespace sm
} // namespace boz