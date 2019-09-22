#include <iostream>

#include "window.hpp"
#include "shaders.hpp"

#include <fstream>
#include <streambuf>

int main() {
    auto window = Window{"Test", 800, 800};
    window.foo();

    std::ifstream vs_file( "../shaders/vertex_test.glsl" );
    std::string vs_source{ std::istreambuf_iterator<char>(vs_file), std::istreambuf_iterator<char>() };
    vs_file.close();
    Shader vertex_shader{Shader::Type::VERTEX, vs_source };

    std::ifstream fs_file( "../shaders/fragment_test.glsl" );
    std::string fs_source{ std::istreambuf_iterator<char>(fs_file), std::istreambuf_iterator<char>() };
    fs_file.close();
    Shader fragment_shader{Shader::Type::FRAGMENT, fs_source };

    ShaderProgram program{vertex_shader, fragment_shader};

    return 0;
}
