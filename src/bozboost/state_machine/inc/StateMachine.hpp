#pragma once 

#include "State.hpp"

namespace boz {
namespace sm {
    
class StateMachine : public State {

public:
    explicit StateMachine(State *parent=0);
    ~StateMachine();
    
    void start();
    void stop();

protected:
    virtual void onEntry();
    virtual void onExit();
    
private:
    BaseState* _initState;
};

} // namespace sm
} // namespace boz