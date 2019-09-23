#include "shaders.hpp"

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
}

Shader::~Shader() {
    if( _shader_id_shared.unique() ) {
        glDeleteShader( *_shader_id_shared );
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

    _program_id_shared.reset(new GLuint{program_id});
}

ShaderProgram::~ShaderProgram() {
    if( _program_id_shared.unique() ) {
        glDeleteShader( *_program_id_shared );
    }
}


void ShaderProgram::use() const {
    glUseProgram(*_program_id_shared);
}
