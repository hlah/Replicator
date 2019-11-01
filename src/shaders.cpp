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

ShaderProgram::ShaderProgram(std::initializer_list<Shader> shaders) {
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

std::shared_ptr<ShaderProgram> ShaderProgramLoader::load( const std::string& vs_filename, const std::string fs_filename ) const {
    auto vs_source = _load_file( vs_filename );
    auto fs_source = _load_file( fs_filename );
    auto vs = Shader{ Shader::Type::VERTEX, vs_source };
    auto fs = Shader{ Shader::Type::FRAGMENT, fs_source };

    std::shared_ptr<ShaderProgram> program{ 
        new ShaderProgram{ vs, fs } ,
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
    delete program;
}
