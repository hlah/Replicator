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

            auto program_handle = program_cache.load<ShaderProgramLoader>(
                    "shader_program"_hs, 
                    "../shaders/vertex_test.glsl", 
                    "../shaders/fragment_test.glsl" 
            );


            _machine = registry.create();
            registry.assign<Transform>( _machine, matrix_op::translation( 0.0, -2.0, -4.0 ) );
            registry.assign<Hierarchy>( _machine );

            _base = registry.create();
            registry.assign<Model>( _base, mesh, program_handle );
            registry.assign<Transform>( _base, matrix_op::translation(0.0, 0.2, 0.0) * matrix_op::scale( 2.0, 0.4, 2.0 ) );
            registry.assign<Hierarchy>( _base, _machine );

            _arm_level = registry.create();
            registry.assign<Transform>( _arm_level, matrix_op::translation(0.0, 0.2, 0.0) );
            registry.assign<Hierarchy>( _arm_level, _machine );

            _arm = registry.create();
            registry.assign<Model>( _arm, mesh, program_handle );
            registry.assign<Transform>( _arm, matrix_op::translation(0.0, 1.0, 0.0) * matrix_op::scale( 0.5, 2.0, 0.5 ) );
            registry.assign<Hierarchy>( _arm, _arm_level );

            _forearm_level = registry.create();
            registry.assign<Transform>( _forearm_level, matrix_op::translation(0.0, 2.0, 0.0) );
            registry.assign<Hierarchy>( _forearm_level, _arm_level );

            _forearm = registry.create();
            registry.assign<Model>( _forearm, mesh, program_handle );
            registry.assign<Transform>( _forearm, matrix_op::translation(0.0, 0.5, 0.0) * matrix_op::scale( 0.4, 2.0, 0.4 ) );
            registry.assign<Hierarchy>( _forearm, _forearm_level );

            _camera = registry.create();
            registry.assign<Camera>( _camera, (float)M_PI/2.f, -0.1f, -10.0f );
            registry.assign<Transform>( _camera, matrix_op::translation(0.0, 0.0, 5.0) );
            registry.assign<Hierarchy>( _camera );
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
            if( action.name() == "CameraMoveH" ) {
                if ( action.type() == ActionEvent::Type::ON  ) {
                    _cam_horizontal_v += _cam_speed;
                }
                if ( action.type() == ActionEvent::Type::OFF  ) {
                    _cam_horizontal_v -= _cam_speed;
                }
            }
            if( action.name() == "CameraMoveHInv" ) {
                if ( action.type() == ActionEvent::Type::ON  ) {
                    _cam_horizontal_v -= _cam_speed;
                }
                if ( action.type() == ActionEvent::Type::OFF  ) {
                    _cam_horizontal_v += _cam_speed;
                }
            }
            return State::Transition::NONE;
        }

        virtual Transition update( entt::registry& registry ) override {
            auto new_arm_transform 
                = matrix_op::rotate_y( _arm_rotation )
                * registry.get<Transform>( _arm_level ).local;
            registry.replace<Transform>( _arm_level, new_arm_transform );

            auto new_forearm_transform 
                = registry.get<Transform>( _forearm_level ).local
                * matrix_op::rotate_z( _forearm_rotation );
            registry.replace<Transform>( _forearm_level, new_forearm_transform );

            auto new_camera_transform
                = matrix_op::translation( _cam_horizontal_v, 0.0, 0.0 )
                * registry.get<Transform>( _camera ).local;
            registry.replace<Transform>( _camera, new_camera_transform );



            transform_system( registry );
            camera_system( registry );
            model_system( registry );

            return State::Transition::NONE;
        }
    private:
        entt::entity _camera;
        entt::entity _machine, _base, _arm_level, _arm, _forearm_level, _forearm;

        const float _arm_rotation_speed = 0.05;
        float _arm_rotation = 0.0;

        const float _forearm_rotation_speed = 0.05;
        float _forearm_rotation = 0.0;

        const float _cam_speed = 0.1;
        float _cam_horizontal_v = 0.0;
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

    auto move_camera_h_action_id  = engine.get_action_id( "CameraMoveH" );
    engine.bind_key( Key::Right, move_camera_h_action_id );
    auto move_camera_hinv_action_id  = engine.get_action_id( "CameraMoveHInv" );
    engine.bind_key( Key::Left, move_camera_hinv_action_id );

    MyState state;
    engine.run(&state);
    
    return 0;
}
