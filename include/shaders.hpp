#ifndef _REPLICATOR_SHADERS_HPP_
#define _REPLICATOR_SHADERS_HPP_

#include "glad/glad.h"

#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <string>
#include <memory>
#include <vector>

class Shader {
    public:
        // Shader types
        enum class Type: GLenum {
            VERTEX = GL_VERTEX_SHADER,
            FRAGMENT = GL_FRAGMENT_SHADER
        };

        // Create a new shader from string
        Shader(Type type, std::string& source);

        ~Shader();

        // Conversion to GLint
        operator GLuint () const { return *_shader_id_shared; }

    private:
        std::shared_ptr<GLuint> _shader_id_shared;
};

class ShaderProgram {
    public:
        // Create new program from multiple shaders
        ShaderProgram(std::initializer_list<Shader>);

        ~ShaderProgram();

        // Use shader program
        void use() const;

        // Set uniform value
        void uniform( const std::string name, const glm::mat4& value ) {
            auto location = glGetUniformLocation( *_program_id_shared, name.c_str() );
            _uniforms_to_set.push_back( { location, value } );
        }

    private:
        std::shared_ptr<GLuint> _program_id_shared;

        // For uniform setting
        struct uniform_pair {
            GLint location;
            glm::mat4 value;
        };

        mutable std::vector<uniform_pair> _uniforms_to_set;

};

class ShaderException : public std::exception {
    public:
        ShaderException( const std::string& msg ) : _msg{msg} { }
        const char* what() const throw() { return _msg.c_str(); }

    private:
        const std::string _msg;
};

#endif // _REPLICATOR_SHADERS_HPP_


