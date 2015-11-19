#pragma once 

#include "BaseState.hpp"

namespace boz {
namespace sm {
    

class State : public BaseState {

public:
    explicit State(State *parent=0);
    ~State();
    
    BaseState* initialState() const;
    void setInitialState(BaseState* state);

protected:
    virtual void onEntry();
    virtual void onExit();
    
private:
    BaseState* _initState;
};

} // namespace sm
} // namespace boz