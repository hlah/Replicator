#include "engine.hpp"
#include "state.hpp"
#include "mesh.hpp"
#include "models.hpp"
#include "transform.hpp"
#include "camera.hpp"
#include "hierarchy.hpp"

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
            mb.cube( 1.0 );
            mb.add_color( 1.0, 0.0, 0.0 );
            mb.add_color( 0.0, 0.0, 1.0 );
            mb.add_color( 0.0, 0.0, 1.0 );
            mb.add_color( 1.0, 0.0, 0.0 );
            mb.add_color( 1.0, 0.0, 0.0 );
            mb.add_color( 0.0, 0.0, 1.0 );
            mb.add_color( 0.0, 0.0, 1.0 );
            mb.add_color( 1.0, 0.0, 0.0 );
            auto mesh = mb.build();

            MeshBuilder mb2;
            mb2.add_vertex( -1.0, 0.0, -1.0 );
            mb2.add_vertex( 1.0, 0.0, -1.0 );
            mb2.add_vertex( 1.0, 0.0, 1.0 );
            mb2.add_vertex( -1.0, 0.0, 1.0 );
            mb2.add_color( 0.0, 0.8, 0.0 );
            mb2.add_color( 0.0, 0.8, 0.0 );
            mb2.add_color( 0.0, 0.8, 0.0 );
            mb2.add_color( 0.0, 0.8, 0.0 );
            mb2.add_index( 0 ); mb2.add_index( 2 ); mb2.add_index( 1 );
            mb2.add_index( 3 ); mb2.add_index( 2 ); mb2.add_index( 0 );
            auto mesh2 = mb2.build();

            MeshBuilder mb3;
            mb3.add_vertex( -1.0, 0.0, -1.0 );
            mb3.add_vertex( 1.0, 0.0, -1.0 );
            mb3.add_vertex( 1.0, 0.0, 1.0 );
            mb3.add_vertex( -1.0, 0.0, 1.0 );
            mb3.add_color( 0.0, 0.5, 0.0 );
            mb3.add_color( 0.0, 0.5, 0.0 );
            mb3.add_color( 0.0, 0.5, 0.0 );
            mb3.add_color( 0.0, 0.5, 0.0 );
            mb3.add_index( 0 ); mb3.add_index( 2 ); mb3.add_index( 1 );
            mb3.add_index( 3 ); mb3.add_index( 2 ); mb3.add_index( 0 );
            auto mesh3 = mb3.build();

            auto program_handle = program_cache.load<ShaderProgramLoader>(
                    "shader_program"_hs, 
                    "../shaders/vertex_test.glsl", 
                    "../shaders/fragment_test.glsl" 
            );

            /// Create Terrain
            for( int i = -5; i<=5; i++ ) {
                for( int j = -5; j<=5; j++ ) {
                    auto terrain = registry.create();
                    registry.assign<Transform>( terrain, Transform{}.translate( 2*i, -2.0, 2*j ) );
                    registry.assign<Hierarchy>( terrain );
                    if( (i+j)%2 == 0 ) {
                        registry.assign<Model>( terrain, mesh2, program_handle );
                    } else {
                        registry.assign<Model>( terrain, mesh3, program_handle );
                    }
                }
            }

            //// Create machine /////
            _machine = registry.create();
            registry.assign<Transform>( _machine, Transform{}.translate(0.0, -2.0, 0.0));
            registry.assign<Hierarchy>( _machine );

            _base = registry.create();
            registry.assign<Model>( _base, mesh, program_handle );
            registry.assign<Transform>( _base, Transform{}.translate(0.0, 0.2, 0.0).scale(2.0, 0.4, 2.0) );
            registry.assign<Hierarchy>( _base, _machine );

            _arm_level = registry.create();
            registry.assign<Transform>( _arm_level, Transform{}.translate(0.0, 0.2, 0.0) );
            registry.assign<Hierarchy>( _arm_level, _machine );

            _arm = registry.create();
            registry.assign<Model>( _arm, mesh, program_handle );
            registry.assign<Transform>( _arm, Transform{}.translate(0.0, 1.0, 0.0).scale( 0.5, 2.0, 0.5 ) );
            registry.assign<Hierarchy>( _arm, _arm_level );

            _forearm_level = registry.create();
            registry.assign<Transform>( _forearm_level, Transform{}.translate(0.0, 2.0, 0.0) );
            registry.assign<Hierarchy>( _forearm_level, _arm_level );

            _forearm = registry.create();
            registry.assign<Model>( _forearm, mesh, program_handle );
            registry.assign<Transform>( _forearm, Transform{}.translate(0.0, 0.5, 0.0).scale( 0.4, 2.0, 0.4 ) );
            registry.assign<Hierarchy>( _forearm, _forearm_level );

            //// Create player with camera
            _player = registry.create();
            registry.assign<Transform>( _player, Transform{}.translate(0.0, 0.0, 5.0) );
            registry.assign<Hierarchy>( _player );

            _camera = registry.create();
            registry.assign<Camera>( _camera, (float)M_PI/2.f, -0.1f, -50.0f );
            registry.assign<Transform>( _camera );
            registry.assign<Hierarchy>( _camera, _player );
            registry.set<CurrentCamera>( _camera );

            return State::Transition::NONE;
        }

        virtual Transition on_action( entt::registry& registry, const ActionEvent& action ) override {
            if( action.name() == "Close" && action.type() == ActionEvent::Type::ON ) {
                return State::Transition::QUIT;
            }
            if( action.name() == "RotateArm" ) {
                if ( action.type() == ActionEvent::Type::ON  ) {
                    _arm_rotation += _arm_rotation_speed;
                }
                if ( action.type() == ActionEvent::Type::OFF  ) {
                    _arm_rotation -= _arm_rotation_speed;
                }
            }
            if( action.name() == "RotateArmInv" ) {
                if ( action.type() == ActionEvent::Type::ON  ) {
                    _arm_rotation -= _arm_rotation_speed;
                }
                if ( action.type() == ActionEvent::Type::OFF  ) {
                    _arm_rotation += _arm_rotation_speed;
                }
            }
            if( action.name() == "RotateForeArm" ) {
                if ( action.type() == ActionEvent::Type::ON  ) {
                    _forearm_rotation += _forearm_rotation_speed;
                }
                if ( action.type() == ActionEvent::Type::OFF  ) {
                    _forearm_rotation -= _forearm_rotation_speed;
                }
            }
            if( action.name() == "RotateForeArmInv" ) {
                if ( action.type() == ActionEvent::Type::ON  ) {
                    _forearm_rotation -= _forearm_rotation_speed;
                }
                if ( action.type() == ActionEvent::Type::OFF  ) {
                    _forearm_rotation += _forearm_rotation_speed;
                }
            }
            if( action.name() == "CameraMoveRight" ) {
                if ( action.type() == ActionEvent::Type::ON  ) {
                    _player_horizontal_v += _player_speed;
                }
                if ( action.type() == ActionEvent::Type::OFF  ) {
                    _player_horizontal_v -= _player_speed;
                }
            }
            if( action.name() == "CameraMoveLeft" ) {
                if ( action.type() == ActionEvent::Type::ON  ) {
                    _player_horizontal_v -= _player_speed;
                }
                if ( action.type() == ActionEvent::Type::OFF  ) {
                    _player_horizontal_v += _player_speed;
                }
            }
            if( action.name() == "CameraMoveForward" ) {
                if ( action.type() == ActionEvent::Type::ON  ) {
                    _player_transversal_v -= _player_speed;
                }
                if ( action.type() == ActionEvent::Type::OFF  ) {
                    _player_transversal_v += _player_speed;
                }
            }
            if( action.name() == "CameraMoveBackward" ) {
                if ( action.type() == ActionEvent::Type::ON  ) {
                    _player_transversal_v += _player_speed;
                }
                if ( action.type() == ActionEvent::Type::OFF  ) {
                    _player_transversal_v -= _player_speed;
                }
            }
            if( action.name() == "CameraRotateRight" ) {
                if ( action.type() == ActionEvent::Type::ON  ) {
                    _player_y_rotation -= _player_rotation_speed;
                }
                if ( action.type() == ActionEvent::Type::OFF  ) {
                    _player_y_rotation += _player_rotation_speed;
                }
            }
            if( action.name() == "CameraRotateLeft" ) {
                if ( action.type() == ActionEvent::Type::ON  ) {
                    _player_y_rotation += _player_rotation_speed;
                }
                if ( action.type() == ActionEvent::Type::OFF  ) {
                    _player_y_rotation -= _player_rotation_speed;
                }
            }
            if( action.name() == "CameraRotateUp" ) {
                if ( action.type() == ActionEvent::Type::ON  ) {
                    _cam_x_rotation -= _player_rotation_speed;
                }
                if ( action.type() == ActionEvent::Type::OFF  ) {
                    _cam_x_rotation += _player_rotation_speed;
                }
            }
            if( action.name() == "CameraRotateDown" ) {
                if ( action.type() == ActionEvent::Type::ON  ) {
                    _cam_x_rotation += _player_rotation_speed;
                }
                if ( action.type() == ActionEvent::Type::OFF  ) {
                    _cam_x_rotation -= _player_rotation_speed;
                }
            }
            return State::Transition::NONE;
        }

        virtual Transition update( entt::registry& registry ) override {
            auto new_arm_transform = registry.get<Transform>( _arm_level );
            new_arm_transform.rotate_y( _arm_rotation );
            registry.replace<Transform>( _arm_level, new_arm_transform );

            auto new_forearm_transform = registry.get<Transform>( _forearm_level );
            new_forearm_transform.rotate_z( _forearm_rotation );
            registry.replace<Transform>( _forearm_level, new_forearm_transform );

            auto new_player_transform = registry.get<Transform>( _player );
            new_player_transform.translate(_player_horizontal_v, 0.0, _player_transversal_v);
            new_player_transform.rotate_y( _player_y_rotation );
            registry.replace<Transform>( _player, new_player_transform );

            auto new_cam_transform = registry.get<Transform>( _camera );
            new_cam_transform.rotate_x( _cam_x_rotation );
            registry.replace<Transform>( _camera, new_cam_transform );

            transform_system( registry );
            camera_system( registry );
            model_system( registry );

            return State::Transition::NONE;
        }
    private:
        entt::entity _player, _camera;
        entt::entity _machine, _base, _arm_level, _arm, _forearm_level, _forearm;

        const float _arm_rotation_speed = 0.05;
        float _arm_rotation = 0.0;

        const float _forearm_rotation_speed = 0.05;
        float _forearm_rotation = 0.0;

        const float _player_rotation_speed = 0.05;
        float _player_y_rotation = 0.0;
        float _cam_x_rotation = 0.0;

        const float _player_speed = 0.1;
        float _player_horizontal_v = 0.0;
        float _player_transversal_v = 0.0;
};

int main() {

    auto engine = Engine{};
    engine.set_window_size(800, 600);
    engine.set_window_title("Replicator");

    auto close_action_id = engine.get_action_id( "Close" );
    engine.bind_key( Key::Escape, close_action_id );

    auto rotate_arm_action_id  = engine.get_action_id( "RotateArm" );
    engine.bind_key( Key::A, rotate_arm_action_id );
    auto rotate_arm_inv_action_id  = engine.get_action_id( "RotateArmInv" );
    engine.bind_key( Key::D, rotate_arm_inv_action_id );
    auto rotate_forearm_action_id  = engine.get_action_id( "RotateForeArm" );
    engine.bind_key( Key::S, rotate_forearm_action_id );
    auto rotate_forearm_inv_action_id  = engine.get_action_id( "RotateForeArmInv" );
    engine.bind_key( Key::W, rotate_forearm_inv_action_id );

    auto move_camera_right_action_id  = engine.get_action_id( "CameraMoveRight" );
    engine.bind_key( Key::KeyPad6, move_camera_right_action_id );
    auto move_camera_left_action_id  = engine.get_action_id( "CameraMoveLeft" );
    engine.bind_key( Key::KeyPad4, move_camera_left_action_id );

    auto move_camera_forward_action_id  = engine.get_action_id( "CameraMoveForward" );
    engine.bind_key( Key::KeyPad8, move_camera_forward_action_id );
    auto move_camera_backward_action_id  = engine.get_action_id( "CameraMoveBackward" );
    engine.bind_key( Key::KeyPad2, move_camera_backward_action_id );

    auto rotate_camera_right_action_id  = engine.get_action_id( "CameraRotateRight" );
    engine.bind_key( Key::KeyPad3, rotate_camera_right_action_id );
    auto rotate_camera_left_action_id  = engine.get_action_id( "CameraRotateLeft" );
    engine.bind_key( Key::KeyPad1, rotate_camera_left_action_id );

    auto rotate_camera_up_action_id  = engine.get_action_id( "CameraRotateUp" );
    engine.bind_key( Key::Up, rotate_camera_up_action_id );
    auto rotate_camera_down_action_id  = engine.get_action_id( "CameraRotateDown" );
    engine.bind_key( Key::Down, rotate_camera_down_action_id );

    MyState state;
    engine.run(&state);
    
    return 0;
}
