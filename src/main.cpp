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

            _cam_id = world.create_object( world.root() );
            auto& cam = world.get_object( _cam_id );
            cam.set_model_transform( matrix_op::translation( 0.0, 0.0, 2.0 ) );
            world.set_camera( _cam_id );

            return State::Transition::NONE;
        }

        virtual Transition on_action( const ActionEvent& action, World& world ) {
            if( action.name() == "Close" && action.type() == ActionEvent::Type::ON ) {
                return State::Transition::QUIT;
            }
            if( action.name() == "MoveLeft" && action.type() == ActionEvent::Type::ON ) {
                auto& cam = world.get_object( 2 );
                cam.set_model_transform( 
                        matrix_op::translation( -1.0, 0.0, 0.0 ) * (*cam.get_model_transform())
                );
            }
            if( action.name() == "MoveRight" && action.type() == ActionEvent::Type::ON ) {
                auto& cam = world.get_object( 2 );
                cam.set_model_transform( 
                        matrix_op::translation( 1.0, 0.0, 0.0 ) * (*cam.get_model_transform())
                );
            }
            if( action.name() == "MoveForward" && action.type() == ActionEvent::Type::ON ) {
                auto& cam = world.get_object( 2 );
                cam.set_model_transform( 
                        matrix_op::translation( 0.0, 0.0, -1.0 ) * (*cam.get_model_transform())
                );
            }
            if( action.name() == "MoveBackward" && action.type() == ActionEvent::Type::ON ) {
                auto& cam = world.get_object( 2 );
                cam.set_model_transform( 
                        matrix_op::translation( 0.0, 0.0, 1.0 ) * (*cam.get_model_transform())
                );
            }

            return State::Transition::NONE;
        }

    private:
        ObjectId _cam_id;
};

int main() {

    auto engine = Engine{};
    engine.set_window_size(800, 600);
    engine.set_window_title("Replicator");

    auto left_action_id = engine.get_action_id( "MoveLeft" );
    engine.bind_key( Key::Left, left_action_id );
    auto right_action_id = engine.get_action_id( "MoveRight" );
    engine.bind_key( Key::Right, right_action_id );
    auto forward_action_id = engine.get_action_id( "MoveForward" );
    engine.bind_key( Key::Up, forward_action_id );
    auto backward_action_id = engine.get_action_id( "MoveBackward" );
    engine.bind_key( Key::Down, backward_action_id );

    auto close_action_id = engine.get_action_id( "Close" );
    engine.bind_key( Key::Escape, close_action_id );


    MyState state;

    engine.run(&state);
    
    return 0;
}
