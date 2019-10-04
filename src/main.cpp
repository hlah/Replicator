#include "engine.hpp"
#include "state.hpp"
#include "mesh.hpp"
#include "models.hpp"

#include "matrix_op.hpp"

#include "glm/mat4x4.hpp"

#include "entt/entt.hpp"

#include <iostream>
#include <fstream>
#include <streambuf>

class MyState : public State {
    public:
        virtual Transition on_start( entt::registry& registry ) override {
            MeshBuilder mb;
            mb.add_vertex( 0.0, 0.5, 0.0 );
            mb.add_color( 1.0, 0.0, 0.0 );
            mb.add_vertex( 0.5, -0.5, 0.0 );
            mb.add_color( 0.0, 1.0, 0.0 );
            mb.add_vertex( -0.5, -0.5, 0.0 );
            mb.add_color( 0.0, 0.0, 1.0 );
            auto mesh = mb.build();

            std::ifstream vs_file( "../shaders/vertex_test.glsl" );
            std::string vs_source{ std::istreambuf_iterator<char>(vs_file), std::istreambuf_iterator<char>() };
            vs_file.close();
            Shader vertex_shader{Shader::Type::VERTEX, vs_source };

            std::ifstream fs_file( "../shaders/fragment_test.glsl" );
            std::string fs_source{ std::istreambuf_iterator<char>(fs_file), std::istreambuf_iterator<char>() };
            fs_file.close();
            Shader fragment_shader{Shader::Type::FRAGMENT, fs_source };

            ShaderProgram program{vertex_shader, fragment_shader};

            auto triangle = registry.create();
            registry.assign<Model>( triangle, mesh, program );

            return State::Transition::NONE;
        }

        virtual Transition on_action( entt::registry& registry, const ActionEvent& action ) override {
            if( action.name() == "Close" && action.type() == ActionEvent::Type::ON ) {
                return State::Transition::QUIT;
            }
            return State::Transition::NONE;
        }

        virtual Transition update( entt::registry& registry ) override {
            model_system( registry );
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
