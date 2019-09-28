#include "engine.hpp"
#include "state.hpp"
#include "mesh.hpp"

#include "matrix_op.hpp"

#include "glm/mat4x4.hpp"

#include <iostream>
#include <fstream>
#include <streambuf>

class MyState : public State {
    public:
        virtual Transition on_start(World &world) {
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

            auto obj_id = world.create_object( world.root() );
            auto& obj = world.get_object( obj_id );
            obj.set_mesh( mesh );
            obj.set_shader_program( program );
            obj.set_model_transform( matrix_op::translation( 0.0, 0.0, -2.0 ) );

            return State::Transition::NONE;
        }

        virtual Transition on_action( const ActionEvent& action ) {
            if( action.name() == "Jump" && action.type() == ActionEvent::Type::ON ) {
                spdlog::info("Jumping!");
            }
            if( action.name() == "Close" && action.type() == ActionEvent::Type::ON ) {
                return State::Transition::QUIT;
            }

            return State::Transition::NONE;
        }

    private:
        int _counter = 3;
};

int main() {

    auto engine = Engine{};
    engine.set_window_size(800, 600);
    engine.set_window_title("Replicator");

    auto jump_action_id = engine.get_action_id( "Jump" );
    engine.bind_key( Key::Space, jump_action_id );

    auto close_action_id = engine.get_action_id( "Close" );
    engine.bind_key( Key::Escape, close_action_id );


    MyState state;

    engine.run(&state);
    
    return 0;
}
