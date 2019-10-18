#include "engine.hpp"
#include "state.hpp"
#include "mesh.hpp"
#include "models.hpp"
#include "transform.hpp"
#include "camera.hpp"

#include "matrix_op.hpp"

#include "glm/mat4x4.hpp"

#include "entt/entt.hpp"

#include <iostream>
#include <fstream>
#include <streambuf>

class MyState : public State {
    public:
        virtual Transition on_start( entt::registry& registry ) override {
            auto& program_cache = registry.set<entt::resource_cache<ShaderProgram>>();

            MeshBuilder mb;
            mb.add_vertex( 0.0, 0.5, 0.0 );
            mb.add_color( 1.0, 0.0, 0.0 );
            mb.add_vertex( -0.5, -0.5, 0.0 );
            mb.add_color( 0.0, 0.0, 1.0 );
            mb.add_vertex( 0.5, -0.5, 0.0 );
            mb.add_color( 0.0, 1.0, 0.0 );
            auto mesh = mb.build();

            auto program_handle = program_cache.load<ShaderProgramLoader>(
                    "shader_program"_hs, 
                    "../shaders/vertex_test.glsl", 
                    "../shaders/fragment_test.glsl" 
            );

            _triangle = registry.create();
            registry.assign<Model>( _triangle, mesh, program_handle );
            registry.assign<Transform>( _triangle, matrix_op::translation( 0.0, 0.0, -_s ) );

            registry.set<Camera>( (float)M_PI/2.f, -0.1f, -10.0f );

            return State::Transition::NONE;
        }

        virtual Transition on_action( entt::registry& registry, const ActionEvent& action ) override {
            if( action.name() == "Close" && action.type() == ActionEvent::Type::ON ) {
                return State::Transition::QUIT;
            }
            return State::Transition::NONE;
        }

        virtual Transition update( entt::registry& registry ) override {
            _s += 0.01;
            registry.get<Transform>(_triangle) = Transform{ matrix_op::translation( 0.0, 0.0, -_s ) };
            //spdlog::debug("{}", _s);

            transform_system( registry );
            camera_system( registry );
            model_system( registry );
            return State::Transition::NONE;
        }
    private:
        float _s = 1.0;
        entt::entity _triangle{};
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
