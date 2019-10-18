#ifndef _REPLICATOR_SHADERS_HPP_
#define _REPLICATOR_SHADERS_HPP_

#include "glad/glad.h"

#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "entt/entt.hpp"

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

class ShaderProgramDeleter;

class ShaderProgram {
    public:
        // Create new program from multiple shaders
        ShaderProgram(std::initializer_list<Shader>);

        // Use shader program
        void use() const;

        // Set uniform value
        void uniform( const std::string name, const glm::mat4& value ) {
            auto location = glGetUniformLocation( _program_id, name.c_str() );
            _uniforms_to_set[ location ] = value;
        }

    private:
        GLuint _program_id;

        mutable std::unordered_map<GLint, glm::mat4> _uniforms_to_set;

        friend class ShaderProgramDeleter;
};

class ShaderProgramDeleter {
    public:
        void operator()(ShaderProgram* program);
};

class ShaderException : public std::exception {
    public:
        ShaderException( const std::string& msg ) : _msg{msg} { }
        const char* what() const throw() { return _msg.c_str(); }

    private:
        const std::string _msg;
};

class ShaderProgramLoader final: public entt::resource_loader<ShaderProgramLoader, ShaderProgram> {
    public:
        std::shared_ptr<ShaderProgram> load( const std::string& vs_filename, const std::string fs_filename ) const;
    private:
        std::string _load_file( const std::string& path ) const;
};

#endif // _REPLICATOR_SHADERS_HPP_


