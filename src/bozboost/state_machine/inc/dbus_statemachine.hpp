#include <iostream>
// back-end
#include <boost/msm/back/state_machine.hpp>
//front-end
#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>

using namespace std;
namespace msm = boost::msm;
using namespace msm::front;
namespace mpl = boost::mpl;

namespace boz {
    // events
    struct event1 {};
    struct event2 {};
    struct event3 {};

    // front-end: define the FSM structure 
    struct dbus_sm_ : public msm::front::state_machine_def<dbus_sm_>
    {
        // The list of FSM states
        struct enter : public msm::front::state<> 
        {
            // every (optional) entry/exit methods get the event passed.
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& );
            template <class Event,class FSM>
            void on_exit(Event const&,FSM& );
        };
        struct state1 : public msm::front::state<> 
        {
            // every (optional) entry/exit methods get the event passed.
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& );
            template <class Event,class FSM>
            void on_exit(Event const&,FSM& );
        };
        struct state2 : public msm::front::state<> 
        {
            // every (optional) entry/exit methods get the event passed.
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& );
            template <class Event,class FSM>
            void on_exit(Event const&,FSM& );
        };
        struct state3 : public msm::front::state<> 
        {
            // every (optional) entry/exit methods get the event passed.
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& );
            template <class Event,class FSM>
            void on_exit(Event const&,FSM& );
        };
        struct exit : public msm::front::state<> 
        { 
            template <class Event,class FSM>
            void on_entry(Event const& ,FSM&) {std::cout << "entering: Open" << std::endl;}
            template <class Event,class FSM>
            void on_exit(Event const&,FSM& ) {std::cout << "leaving: Open" << std::endl;}
        };

        // the initial state of the player SM. Must be defined
        typedef enter initial_state;

        // transition actions
        // as the functors are generic on events, fsm and source/target state, 
        // you can reuse them in another machine if you wish
        struct toState1 
        {
            template <class EVT,class FSM,class SourceState,class TargetState>
            void operator()(EVT const& ,FSM& ,SourceState& ,TargetState& )
            {
                cout << "dbus_sm::toState1" << endl;
            }
        };
        struct toState2 
        {
            template <class EVT,class FSM,class SourceState,class TargetState>
            void operator()(EVT const& ,FSM& ,SourceState& ,TargetState& )
            {
                cout << "dbus_sm::toState2" << endl;
            }
        };
        struct toState3 
        {
            template <class EVT,class FSM,class SourceState,class TargetState>
            void operator()(EVT const& ,FSM& ,SourceState& ,TargetState& )
            {
                cout << "dbus_sm::toState3" << endl;
            }
        };
        struct toExit
        {
            template <class EVT,class FSM,class SourceState,class TargetState>
            void operator()(EVT const& ,FSM& ,SourceState& ,TargetState& )
            {
                cout << "dbus_sm::toExit" << endl;
            }
        };
        // Transition table for player
        struct transition_table : mpl::vector<
            //    Start       Event       Next        Action          Guard
            //  +-----------+-----------+-----------+---------------+--------+
            Row < enter,    none,       state1,     toState1,       none    >,
            Row < state1,   event1,     state2,     toState2,       none    >,
            Row < state2,   event2,     state3,     toState3,       none    >,
            Row < state3,   event3,     exit,       toExit,         none    >
            //  +-----------+-----------+-----------+---------------+--------+
        > {};
        // Replaces the default no-transition response.
        template <class FSM,class Event>
        void no_transition(Event const& e, FSM&,int state)
        {
            std::cout << "no transition from state " << state
                << " on event " << typeid(e).name() << std::endl;
        }
    };
    // Pick a back-end
    typedef msm::back::state_machine<dbus_sm_> dbus_sm;

} // namespace boz