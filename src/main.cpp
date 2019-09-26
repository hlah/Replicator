#include "engine.hpp"
#include "state.hpp"
#include "mesh.hpp"

#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

#include <iostream>
#include <fstream>
#include <streambuf>

class MyState : public State {
    public:
        virtual Transition on_start(World &world) {
            MeshBuilder mb;
            mb.add_vertex( 0.0, 0.5, -0.1 );
            mb.add_color( 1.0, 0.0, 0.0 );
            mb.add_vertex( 0.5, -0.5, -0.1 );
            mb.add_color( 0.0, 1.0, 0.0 );
            mb.add_vertex( -0.5, -0.5, -0.1 );
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
            obj.set_model_transform( glm::scale( glm::vec3{ 1.5, 0.5, 1.0 } ) );

            return State::Transition::NONE;
        }

    private:
        int _counter = 3;
};

int main() {

    auto engine = Engine{};
    engine.set_window_size(800, 600);
    engine.set_window_title("Replicator");

    MyState state;

    engine.run(&state);
    
    return 0;
}
