#include "engine.hpp"
#include "state.hpp"
#include "mesh.hpp"

#include "matrix_op.hpp"

#include "glm/mat4x4.hpp"

#include "entt/entt.hpp"

#include <iostream>
#include <fstream>
#include <streambuf>

void system1( entt::registry& reg ) {
    spdlog::debug("I'm system 1");
}

class MyState : public State {
    public:
        virtual Transition on_action( entt::registry& registry, const ActionEvent& action ) {
            if( action.name() == "Close" && action.type() == ActionEvent::Type::ON ) {
                return State::Transition::QUIT;
            }
            return State::Transition::NONE;
        }

        virtual Transition update( entt::registry& registry ) {
            system1( registry );
            return State::Transition::NONE;
        }
};

int main() {

    auto engine = Engine{};
    engine.set_window_size(800, 600);
    engine.set_window_title("Replicator");

    auto close_action_id = engine.get_action_id( "Close" );
    engine.bind_key( Key::Escape, close_action_id );

    MyState state;
    engine.run(&state);
    
    return 0;
}
