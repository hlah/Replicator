#ifndef _REPLICATOR_STATE_HPP_
#define _REPLICATOR_STATE_HPP_

#include "world.hpp"

class State {
    public:
        // Type of transition for State
        enum class Transition {
            NONE,
            QUIT,
        };

        virtual Transition on_start(World& world) { return Transition::NONE; }
        virtual Transition update() { return Transition::NONE; }
        virtual Transition on_close() { return Transition::QUIT; }
};



#endif // _REPLICATOR_STATE_HPP_
