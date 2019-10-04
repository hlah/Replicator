#ifndef _REPLICATOR_STATE_HPP_
#define _REPLICATOR_STATE_HPP_

#include "world.hpp"
#include "action.hpp"

#include "entt/entt.hpp"

class State {
    public:
        // Type of transition for State
        enum class Transition {
            NONE,
            QUIT,
        };

        virtual Transition on_start( entt::registry& registry ) { return Transition::NONE; }
        virtual Transition update( entt::registry& registry ) { return Transition::NONE; }
        virtual Transition on_close( entt::registry& registry ) { return Transition::QUIT; }
        virtual Transition on_action( entt::registry& registry, const ActionEvent& action ) { return Transition::NONE; }
};



#endif // _REPLICATOR_STATE_HPP_
