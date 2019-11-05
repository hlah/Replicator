#include "engine.hpp"
#include "state.hpp"
#include "mesh.hpp"
#include "models.hpp"
#include "transform.hpp"
#include "camera.hpp"
#include "hierarchy.hpp"
#include "material.hpp"
#include "model_loader.hpp"

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
            auto& texture_cache = registry.set<entt::resource_cache<Texture>>();

            MeshBuilder mb;
            mb.cube( 1.0 );
            auto mesh = mb.build();

            MeshBuilder mb2;
            mb2.rect( glm::vec3{0.0}, glm::vec3{0.0, 0.0, -1.0}, glm::vec3{1.0, 0.0, 0.0} );
            auto mesh2 = mb2.build();

            auto program_handle = program_cache.load<ShaderProgramLoader>(
                    "shader_program"_hs, 
                    std::vector<std::string>{"../shaders/vertex_main.glsl"}, 
                    std::vector<std::string>{
                        "../shaders/fragment_main.glsl", 
                        "../shaders/blinn_phong_shading.glsl", 
                        "../shaders/lights.glsl"
                    } 
            );

            auto model = load_model( registry, "../models/space/source/Sketchfab_2016_01_02_20_11_57.blend.blend", program_handle );
            registry.replace<Transform>( model, Transform{}.scale( 1.0, 1.0, 1.0 ).translate(0.0, -2.0, 0.0).rotate_x( (float)-M_PI/2.f ) );

            auto texture_handle = texture_cache.load<TextureLoader>( 
                    "container_texture"_hs,
                    "../textures/container2.png"
            );

            auto specular_texture_handle = texture_cache.load<TextureLoader>( 
                    "container_diffuse_texture"_hs,
                    "../textures/container2_specular.png"
            );

            /// Create Terrain
            /*
            for( int i = -10; i<=10; i++ ) {
                for( int j = -10; j<=10; j++ ) {
                    auto terrain = registry.create();
                    registry.assign<Transform>( terrain, Transform{}.translate( 2*i, -2.0, 2*j ) );
                    registry.assign<Hierarchy>( terrain );
                    registry.assign<Model>( terrain, mesh2, program_handle );
                    if( (i+j)%2 == 0 ) {
                        registry.assign<Material>( 
                                terrain, 
                                glm::vec3{0.0, 0.0, 0.0}, 
                                glm::vec3{0.5, 0.5, 0.5},  
                                glm::vec3{0.5, 0.5, 0.5},  
                                20.0
                        );
                    } else {
                        registry.assign<Material>( 
                                terrain, 
                                glm::vec3{0.0, 0.0, 0.0}, 
                                glm::vec3{0.0, 0.0, 0.0},  
                                glm::vec3{0.5, 0.5, 0.5},  
                                20.0
                        );
                    }
                }
            }
            */

            //// Create machine /////
            /*
            _machine = registry.create();
            registry.assign<Transform>( _machine, Transform{}.translate(0.0, -2.0, 0.0));
            registry.assign<Hierarchy>( _machine );

            _base = registry.create();
            registry.assign<Model>( _base, mesh, program_handle );
            registry.assign<Transform>( _base, Transform{}.translate(0.0, 0.2, 0.0).scale(2.0, 0.4, 2.0) );
            registry.assign<Hierarchy>( _base, _machine );
            registry.assign<Material>( _base, glm::vec3{0.5, 0.5, 0.5}, 0.02, 0.5, 0.5, 2.0 );

            _arm_level = registry.create();
            registry.assign<Transform>( _arm_level, Transform{}.translate(0.0, 0.2, 0.0) );
            registry.assign<Hierarchy>( _arm_level, _machine );

            _arm = registry.create();
            registry.assign<Model>( _arm, mesh, program_handle );
            registry.assign<Transform>( _arm, Transform{}.translate(0.0, 1.0, 0.0).scale( 0.5, 2.0, 0.5 ) );
            registry.assign<Hierarchy>( _arm, _arm_level );
            registry.assign<Material>( _arm, glm::vec3{0.7, 0.7, 0.7}, 0.02, 0.5, 0.5, 10.0 );

            _forearm_level = registry.create();
            registry.assign<Transform>( _forearm_level, Transform{}.translate(0.0, 2.0, 0.0) );
            registry.assign<Hierarchy>( _forearm_level, _arm_level );

            _forearm = registry.create();
            registry.assign<Model>( _forearm, mesh, program_handle );
            registry.assign<Transform>( _forearm, Transform{}.translate(0.0, 0.5, 0.0).scale( 0.4, 2.0, 0.4 ) );
            registry.assign<Hierarchy>( _forearm, _forearm_level );
            registry.assign<Material>( _forearm, glm::vec3{0.7, 0.3, 0.3}, 0.02, 0.5, 1.5, 20.0 );

            //// Create box
            auto box = registry.create();
            registry.assign<Model>( box, mesh, program_handle );
            registry.assign<Transform>( box, Transform{}.translate(5.0, 0.0, 0.0).scale( 4.0, 4.0, 4.0 ) );
            registry.assign<Hierarchy>( box );
            Material box_material{ glm::vec3{0.0}, 0.0, 0.0, 0.0, 5.0 };
            box_material.add_specular_texture( specular_texture_handle );
            box_material.add_diffuse_texture( texture_handle );
            registry.assign<Material>( box, box_material );
            */

            //// Create player with camera
            _player = registry.create();
            registry.assign<Transform>( _player, Transform{}.translate(0.0, 0.0, 5.0) );
            registry.assign<Hierarchy>( _player );

            _head = registry.create();
            registry.assign<Transform>( _head );
            registry.assign<Hierarchy>( _head, _player );
            registry.set<CurrentCamera>( _head );

            auto camera = registry.create();
            registry.assign<Camera>( camera, (float)M_PI/2.f, -0.1f, -50.0f );
            registry.assign<Transform>( camera );
            registry.assign<Hierarchy>( camera, _head );
            registry.set<CurrentCamera>( camera );

            //// Lights

            auto light = registry.create();
            registry.assign<LightColor>( light, glm::vec3{0.5, 0.5, 0.5} );
            registry.assign<DirectionalLight>( light );
            registry.assign<Transform>( light, Transform{}.rotate_y_global( (float)M_PI/2.f ).rotate_z_global( (float)M_PI/4.f ) );
            registry.assign<Hierarchy>( light );

            auto light2 = registry.create();
            registry.assign<LightColor>( light2, glm::vec3{1.0, 0.2, 1.0} );
            registry.assign<PointLight>( light2 );
            registry.assign<Transform>( light2, Transform{}.translate( -4.0, 1.0, 4.0 ) );
            registry.assign<Hierarchy>( light2 );

            auto light3 = registry.create();
            registry.assign<LightColor>( light3, glm::vec3{0.2, 1.0, 0.2});
            registry.assign<PointLight>( light3 );
            registry.assign<Transform>( light3, Transform{}.translate( 0.0, 0.1, -10.0 ) );
            registry.assign<Hierarchy>( light3 );

            /*
            auto light4 = registry.create();
            registry.assign<LightColor>( light4, glm::vec3{1.5, 1.5, 1.5});
            registry.assign<Spotlight>( light4, (float)M_PI/5.f, (float)M_PI/50.f );
            registry.assign<Transform>( light4 );
            registry.assign<Hierarchy>( light4, _head );
            */


            // Set previous mouse position
            auto window = registry.ctx<std::shared_ptr<Window>>();
            _new_mouse_x = _prev_mouse_x = window->mouse_x();
            _new_mouse_y = _prev_mouse_y = window->mouse_y();

            // Set mouse capture mode
            window->capture_mouse(true);

            return State::Transition::NONE;
        }

        virtual Transition on_action( entt::registry& registry, const ActionEvent& action ) override {
            if( action.name() == "Close" && action.type() == ActionEvent::Type::ON ) {
                return State::Transition::QUIT;
            }

            handle_action( action, "RotateArm", _arm_rotation, _arm_rotation_speed );
            handle_action( action, "RotateArmInv", _arm_rotation, -_arm_rotation_speed );
            handle_action( action, "RotateForeArm", _forearm_rotation, _forearm_rotation_speed );
            handle_action( action, "RotateForeArmInv", _forearm_rotation, -_forearm_rotation_speed );

            handle_action( action, "CameraMoveRight", _player_horizontal_v, _player_speed );
            handle_action( action, "CameraMoveLeft", _player_horizontal_v, -_player_speed );
            handle_action( action, "CameraMoveForward", _player_transversal_v, -_player_speed );
            handle_action( action, "CameraMoveBackward", _player_transversal_v, _player_speed );

            return State::Transition::NONE;
        }

        virtual Transition on_mouse_move( entt::registry& registry, double mx, double my ) override {
            _new_mouse_x = mx;
            _new_mouse_y = my;
            return State::Transition::NONE;
        }

        virtual Transition update( entt::registry& registry ) override {
            _head_x_rotation = -(_new_mouse_y-_prev_mouse_y)*_player_rotation_speed;
            _player_y_rotation = -(_new_mouse_x-_prev_mouse_x)*_player_rotation_speed;

            /*
            auto new_arm_transform = registry.get<Transform>( _arm_level );
            new_arm_transform.rotate_y( _arm_rotation );
            registry.replace<Transform>( _arm_level, new_arm_transform );

            auto new_forearm_transform = registry.get<Transform>( _forearm_level );
            new_forearm_transform.rotate_z( _forearm_rotation );
            registry.replace<Transform>( _forearm_level, new_forearm_transform );
            */

            auto new_player_transform = registry.get<Transform>( _player );
            new_player_transform.translate(_player_horizontal_v, 0.0, _player_transversal_v);
            new_player_transform.rotate_y( _player_y_rotation );
            registry.replace<Transform>( _player, new_player_transform );

            auto new_head_transform = registry.get<Transform>( _head );
            new_head_transform.rotate_x( _head_x_rotation );
            registry.replace<Transform>( _head, new_head_transform );

            transform_system( registry );
            camera_system( registry );
            //material_system( registry );
            light_system( registry );
            model_system( registry );

            _prev_mouse_x = _new_mouse_x;
            _prev_mouse_y = _new_mouse_y;

            return State::Transition::NONE;
        }
    private:
        // Helper function for action that edit float values
        void handle_action( ActionEvent action, const std::string& action_name, float& value_to_edit, float value ) {
            if( action.name() == action_name) {
                if ( action.type() == ActionEvent::Type::ON  ) {
                    value_to_edit += value;
                }
                if ( action.type() == ActionEvent::Type::OFF  ) {
                    value_to_edit -= value;
                }
            }
        }

        entt::entity _player, _head;
        entt::entity _machine, _base, _arm_level, _arm, _forearm_level, _forearm;

        const float _arm_rotation_speed = 0.05;
        float _arm_rotation = 0.0;

        const float _forearm_rotation_speed = 0.05;
        float _forearm_rotation = 0.0;

        const float _player_rotation_speed = 0.005;
        float _player_y_rotation = 0.0;
        float _head_x_rotation = 0.0;

        const float _player_speed = 0.1;
        float _player_horizontal_v = 0.0;
        float _player_transversal_v = 0.0;

        double _prev_mouse_x, _prev_mouse_y;
        double _new_mouse_x, _new_mouse_y;
};

int main() {

    auto engine = Engine{};
    engine.set_window_size(800, 600);
    engine.set_window_title("Replicator");
    engine.set_aa(16);

    engine.bind_key( Key::Escape, "Close" );

    engine.bind_key( Key::A, "RotateArm" );
    engine.bind_key( Key::D, "RotateArmInv" );
    engine.bind_key( Key::S, "RotateForeArm" );
    engine.bind_key( Key::W, "RotateForeArmInv" );

    engine.bind_key( Key::Right, "CameraMoveRight" );
    engine.bind_key( Key::Left, "CameraMoveLeft" );
    engine.bind_key( Key::Up, "CameraMoveForward" );
    engine.bind_key( Key::Down, "CameraMoveBackward" );

    MyState state;
    engine.run(&state);
    
    return 0;
}
