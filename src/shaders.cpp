#include "shaders.hpp"

#include "matrix_op.hpp"

#include "spdlog/spdlog.h"

#include <fstream>

// Shader class
Shader::Shader(Shader::Type type, std::string& source) {
    GLuint shader_id = glCreateShader( (GLuint)type );
    const GLchar* source_ptr = source.c_str();

    // compile
    glShaderSource(shader_id, 1, &source_ptr, NULL);
    glCompileShader( shader_id );

    // check status
    GLint status_ok;
    glGetShaderiv( shader_id, GL_COMPILE_STATUS, &status_ok );
    if( !status_ok ) {
        std::string log_info;

        GLint log_info_len;
        glGetShaderiv( shader_id, GL_INFO_LOG_LENGTH, &log_info_len );
        if( log_info_len > 0 ) {
            char* log_info_cstr = new char[log_info_len];
            glGetShaderInfoLog( shader_id, log_info_len, NULL, log_info_cstr );
            log_info = std::string{ log_info_cstr };
            delete[] log_info_cstr;
        }

        glDeleteShader( shader_id );
        throw ShaderException{ std::string{"Shader compilation error:\n"} + log_info };
    }

    _shader_id_shared.reset(new GLuint{shader_id});
    spdlog::debug("Created shader, id={}", *_shader_id_shared);
}

Shader::~Shader() {
    if( _shader_id_shared.unique() ) {
        glDeleteShader( *_shader_id_shared );
        spdlog::debug("Deleted shader, id={}", *_shader_id_shared);
    }
}

// ShaderProgram class

ShaderProgram::ShaderProgram(std::vector<Shader> shaders) {
    GLuint program_id = glCreateProgram();

    // Link program
    for( const auto& shader : shaders ) {
        glAttachShader( program_id, shader );
    }
    glLinkProgram(program_id);
    for( const auto& shader : shaders ) {
        glDetachShader( program_id, shader );
    }

    // check status
    GLint status_ok;
    glGetProgramiv( program_id, GL_LINK_STATUS, &status_ok );
    if( !status_ok ) {
        std::string log_info;

        GLint log_info_len;
        glGetProgramiv( program_id, GL_INFO_LOG_LENGTH, &log_info_len );
        if( log_info_len > 0 ) {
            char* log_info_cstr = new char[log_info_len];
            glGetProgramInfoLog( program_id, log_info_len, NULL, log_info_cstr );
            log_info = std::string{ log_info_cstr };
            delete[] log_info_cstr;
        }

        glDeleteProgram( program_id );
        throw ShaderException{ std::string{"ShaderProgram link error:\n"} + log_info };
    }

    _program_id = program_id;
    spdlog::debug("Created shader program, id={}", _program_id);
}

/*
ShaderProgram::~ShaderProgram() {
    if( _program_id_shared.unique() ) {
        glDeleteProgram( *_program_id_shared );
        spdlog::debug("Deleted shader program, id={}", *_program_id_shared);
    }
}
*/


void ShaderProgram::use() const {
    glUseProgram(_program_id);
    // set pending uniforms
    // mat4
    for( auto& uniform : _uniforms_to_set_m4 ) {
        glUniformMatrix4fv( uniform.first, 1, GL_FALSE, glm::value_ptr(uniform.second) );
    }
    // vec3
    for( auto& uniform : _uniforms_to_set_v3 ) {
        glUniform3fv( uniform.first, 1, glm::value_ptr(uniform.second) );
    }
    // vec4
    for( auto& uniform : _uniforms_to_set_v4 ) {
        glUniform4fv( uniform.first, 1, glm::value_ptr(uniform.second) );
    }
    // floats
    for( auto uniform : _uniforms_to_set_f ) {
        glUniform1f( uniform.first, uniform.second );
    }
    // uints
    for( auto uniform : _uniforms_to_set_u ) {
        glUniform1ui( uniform.first, uniform.second );
    }
    // ints
    for( auto uniform : _uniforms_to_set_i ) {
        glUniform1i( uniform.first, uniform.second );
    }
    // textures
    for( auto texture : _textures_to_set ) {
        glActiveTexture(GL_TEXTURE0 + texture.first);
        glBindTexture(GL_TEXTURE_2D, texture.second);
    }
    _uniforms_to_set_m4.clear();
    _uniforms_to_set_v4.clear();
    _uniforms_to_set_v3.clear();
    _uniforms_to_set_f.clear();
    _uniforms_to_set_u.clear();
    _uniforms_to_set_i.clear();
    _textures_to_set.clear();
}

void ShaderProgram::uniform( const std::string name, const Material& value ) {
    auto ambient_loc = glGetUniformLocation( _program_id, (name + std::string{".ambient"}).c_str());
    auto diffuse_loc = glGetUniformLocation( _program_id, (name + std::string{".diffuse"}).c_str());
    auto specular_loc = glGetUniformLocation( _program_id, (name + std::string{".specular"}).c_str());
    auto shininess_loc = glGetUniformLocation( _program_id, (name + std::string{".shininess"}).c_str());
    _uniforms_to_set_v3[ ambient_loc ] = value.ambient();
    _uniforms_to_set_v3[ diffuse_loc ] = value.diffuse();
    _uniforms_to_set_v3[ specular_loc ] = value.specular();
    _uniforms_to_set_f[ shininess_loc ] = value.shininess();

    GLuint i = 0;
    unsigned int ambient_texs = 0;
    unsigned int diffuse_texs = 0;
    unsigned int specular_texs = 0;

    // ambient textures
    for( const auto& texture : value.ambient_textures() ) {
        _textures_to_set[i] = *texture;
        auto ambient_texture_loc = glGetUniformLocation( _program_id, (name + std::string{".ambient_textures["} + std::to_string(ambient_texs) + std::string{"]"}).c_str() );
        _uniforms_to_set_i[ ambient_texture_loc ] = i;
        ambient_texs++;
        i++;
    }
    auto ambient_texture_count_loc  = glGetUniformLocation( _program_id, (name + std::string{".ambient_texture_count"}).c_str());
    _uniforms_to_set_u[ ambient_texture_count_loc ] = ambient_texs;

    // diffuse textures
    for( const auto& texture : value.diffuse_textures() ) {
        _textures_to_set[i] = *texture;
        auto diffuse_texture_loc = glGetUniformLocation( _program_id, (name + std::string{".diffuse_textures["} + std::to_string(diffuse_texs) + std::string{"]"}).c_str() );
        _uniforms_to_set_i[ diffuse_texture_loc ] = i;
        diffuse_texs++;
        i++;
    }
    auto diffuse_texture_count_loc  = glGetUniformLocation( _program_id, (name + std::string{".diffuse_texture_count"}).c_str());
    _uniforms_to_set_u[ diffuse_texture_count_loc ] = diffuse_texs;

    // specular textures
    for( const auto& texture : value.specular_textures() ) {
        _textures_to_set[i] = *texture;
        auto specular_texture_loc = glGetUniformLocation( _program_id, (name + std::string{".specular_textures["} + std::to_string(specular_texs) + std::string{"]"}).c_str() );
        _uniforms_to_set_i[ specular_texture_loc ] = i;
        specular_texs++;
        i++;
    }
    auto specular_texture_count_loc  = glGetUniformLocation( _program_id, (name + std::string{".specular_texture_count"}).c_str());
    _uniforms_to_set_u[ specular_texture_count_loc ] = specular_texs;
}

void ShaderProgram::uniform( const std::string& name, const ShaderLight& value ) {
    auto type_loc = glGetUniformLocation( _program_id, (name + std::string{".type"}).c_str());
    auto position_loc = glGetUniformLocation( _program_id, (name + std::string{".position"}).c_str());
    auto direction_loc = glGetUniformLocation( _program_id, (name + std::string{".direction"}).c_str());
    auto color_loc = glGetUniformLocation( _program_id, (name + std::string{".color"}).c_str());
    auto outer_angle_loc = glGetUniformLocation( _program_id, (name + std::string{".outer_angle"}).c_str());
    auto inner_angle_loc = glGetUniformLocation( _program_id, (name + std::string{".inner_angle"}).c_str());
    _uniforms_to_set_u[ type_loc ] = value.type;
    _uniforms_to_set_v4[ position_loc ] = value.position;
    _uniforms_to_set_v4[ direction_loc ] = value.direction;
    _uniforms_to_set_v3[ color_loc ] = value.color;
    _uniforms_to_set_f[ outer_angle_loc ] = std::cos(value.outer_angle);
    _uniforms_to_set_f[ inner_angle_loc ] = std::cos(value.inner_angle);
}

std::shared_ptr<ShaderProgram> ShaderProgramLoader::load( const std::vector<std::string>& vs_paths, const std::vector<std::string>& fs_paths ) const {
    std::vector<Shader> shaders{};
    for( const auto& vs_path : vs_paths ) {
        auto vs_src = _load_file(vs_path);
        shaders.emplace_back( Shader::Type::VERTEX, vs_src );
    }
    for( const auto& fs_path : fs_paths ) {
        auto fs_src = _load_file(fs_path);
        shaders.emplace_back( Shader::Type::FRAGMENT, fs_src );
    }

    std::shared_ptr<ShaderProgram> program{ 
        new ShaderProgram{ shaders } ,
        ShaderProgramDeleter{}
    };

    return program;
}

std::string ShaderProgramLoader::_load_file( const std::string& path ) const {
    std::ifstream file( path );
    std::string source{ std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
    file.close();
    return source;
}


void ShaderProgramDeleter::operator()(ShaderProgram* program) {
    glDeleteProgram( program->_program_id );
    spdlog::debug("Deleted shader program, id={}", program->_program_id);
}
