#include "engine.hpp"
#include "state.hpp"
#include "mesh.hpp"

#include "matrix_op.hpp"

#include "glm/mat4x4.hpp"

#include "entt/entt.hpp"

#include <iostream>
#include <fstream>
#include <streambuf>

class MyState : public State {
    public:
        virtual Transition on_start(World &world) {
            MeshBuilder mb;
            mb.cube( 1.0 );
            //mb.add_vertex( 0.0, 0.5, 0.0 );
            //mb.add_color( 1.0, 0.0, 0.0 );
            //mb.add_vertex( 0.5, -0.5, 0.0 );
            //mb.add_color( 0.0, 1.0, 0.0 );
            //mb.add_vertex( -0.5, -0.5, 0.0 );
            //mb.add_color( 0.0, 0.0, 1.0 );
            mb.add_color( 1.0, 0.0, 0.0 );
            mb.add_color( 1.0, 0.0, 0.0 );
            mb.add_color( 1.0, 0.5, 0.0 );
            mb.add_color( 1.0, 0.5, 0.0 );
            mb.add_color( 0.0, 0.0, 1.0 );
            mb.add_color( 0.0, 0.0, 1.0 );
            mb.add_color( 0.0, 0.5, 1.0 );
            mb.add_color( 0.0, 0.5, 1.0 );
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

            _obj_id = world.create_object( world.root() );
            auto& obj = world.get_object( _obj_id );
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
            if( action.name() == "MoveLeft" ) {
                if( action.type() == ActionEvent::Type::ON ) {
                    _horizontal_v -= _speed;
                } else if( action.type() == ActionEvent::Type::OFF ) {
                    _horizontal_v += _speed;
                }
            }
            if( action.name() == "MoveRight" ) {
                if( action.type() == ActionEvent::Type::ON ) {
                    _horizontal_v += _speed;
                } else if( action.type() == ActionEvent::Type::OFF ) {
                    _horizontal_v -= _speed;
                }
            }
            if( action.name() == "MoveForward" ) {
                if( action.type() == ActionEvent::Type::ON ) {
                    _transversal_v -= _speed;
                } else if( action.type() == ActionEvent::Type::OFF ) {
                    _transversal_v += _speed;
                }
            }
            if( action.name() == "MoveBackward" ) {
                if( action.type() == ActionEvent::Type::ON ) {
                    _transversal_v += _speed;
                } else if( action.type() == ActionEvent::Type::OFF ) {
                    _transversal_v -= _speed;
                }
            }
            if( action.name() == "RotCubeY" && action.type() == ActionEvent::Type::ON ) {
                auto& obj = world.get_object( _obj_id );
                obj.set_model_transform( 
                         (*obj.get_model_transform()) * matrix_op::rotate_y( 0.1f )
                );
            }
            if( action.name() == "RotCubeZ" && action.type() == ActionEvent::Type::ON ) {
                auto& obj = world.get_object( _obj_id );
                obj.set_model_transform( 
                         (*obj.get_model_transform()) * matrix_op::rotate_z( 0.1f )
                );
            }
            if( action.name() == "RotCubeX" && action.type() == ActionEvent::Type::ON ) {
                auto& obj = world.get_object( _obj_id );
                obj.set_model_transform( 
                         (*obj.get_model_transform()) * matrix_op::rotate_x( 0.1f )
                );
            }

            return State::Transition::NONE;
        }
        virtual Transition update( World& world, double delta_time ) {
            auto& cam = world.get_object( _cam_id );
            cam.set_model_transform( 
                    matrix_op::translation( _horizontal_v*delta_time, 0.0, 0.0 ) * (*cam.get_model_transform())
            );
            cam.set_model_transform( 
                    matrix_op::translation( 0.0, 0.0, _transversal_v*delta_time ) * (*cam.get_model_transform())
            );
            return State::Transition::NONE;
        }

    private:
        ObjectId _cam_id;
        ObjectId _obj_id;

        float _horizontal_v = 0.0;
        float _transversal_v = 0.0;
        const float _speed = 2.0;
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

    auto rot_cube_y_action_id = engine.get_action_id( "RotCubeY" );
    engine.bind_key( Key::Y, rot_cube_y_action_id );
    auto rot_cube_z_action_id = engine.get_action_id( "RotCubeZ" );
    engine.bind_key( Key::Z, rot_cube_z_action_id );
    auto rot_cube_x_action_id = engine.get_action_id( "RotCubeX" );
    engine.bind_key( Key::X, rot_cube_x_action_id );


    auto close_action_id = engine.get_action_id( "Close" );
    engine.bind_key( Key::Escape, close_action_id );


    MyState state;

    engine.run(&state);
    
    return 0;
}
