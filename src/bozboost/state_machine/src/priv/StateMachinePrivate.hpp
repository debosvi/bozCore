#pragma once 

#include <boost/mpl/vector.hpp>

namespace boz {
namespace sm {
  
class State;
  
class StateMachinePrivate {
    friend class StateMachine;
    
public:
    explicit StateMachinePrivate();
    ~StateMachinePrivate();

private:
    boost::mpl::vector<State*> _states;
};

} // namespace sm
} // namespace boz