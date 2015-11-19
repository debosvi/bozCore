#pragma once 

namespace boz {
namespace sm {
    
class BaseState;
class State;
class StateMachine;

class BaseTransition {

public:
    explicit BaseTransition(State *source=0);
    ~BaseTransition();
    StateMachine* machine() const;
    State* sourceState() const;
    BaseState* targetState() const;
    void setTargetState(BaseState* target);

protected:
    virtual void onTransition()=0;

private:
    State* _source;
    BaseState* _target;
};

} // namespace sm
} // namespace boz