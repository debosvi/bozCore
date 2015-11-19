#pragma once 

namespace boz {
namespace sm {
    
class State;
class StateMachine;

class BaseState {

public:
    ~BaseState();
    bool active() const;
    StateMachine* machine() const;
    State* parentState() const;

protected:
    explicit BaseState(State *parent=0);
    virtual void onEntry()=0;
    virtual void onExit()=0;

private:
    State* _parent;
    StateMachine* _machine;
    bool _active;
};

} // namespace sm
} // namespace boz