#pragma once 

#include "State.hpp"

namespace boz {
namespace sm {
    
class StateMachinePrivate;
    
class StateMachine : public State {

public:
    explicit StateMachine(State *parent=0);
    ~StateMachine();
    
    void addState(BaseState* state);
    void removeState(BaseState* state);
    
    void start();
    void stop();

protected:
    virtual void onEntry();
    virtual void onExit();
    
private:
    StateMachinePrivate* _members;
};

} // namespace sm
} // namespace boz