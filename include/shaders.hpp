#ifndef _REPLICATOR_SHADERS_HPP_
#define _REPLICATOR_SHADERS_HPP_

#include "material.hpp"
#include "lights.hpp"

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
        ShaderProgram(std::vector<Shader>);

        // Use shader program
        void use() const;

        // Set uniform value
        void uniform( const std::string name, const glm::mat4& value ) {
            auto location = glGetUniformLocation( _program_id, name.c_str() );
            _uniforms_to_set_m4[ location ] = value;
        }
        void uniform( const std::string name, const GLuint value  ) {
            auto location = glGetUniformLocation( _program_id, name.c_str() );
            _uniforms_to_set_u[ location ] = value;
        }
        void uniform( const std::string name, const Material& value ) {
            auto ambient_loc = glGetUniformLocation( _program_id, (name + std::string{".ambient"}).c_str());
            auto diffuse_loc = glGetUniformLocation( _program_id, (name + std::string{".diffuse"}).c_str());
            auto specular_loc = glGetUniformLocation( _program_id, (name + std::string{".specular"}).c_str());
            auto shininess_loc = glGetUniformLocation( _program_id, (name + std::string{".shininess"}).c_str());
            _uniforms_to_set_v3[ ambient_loc ] = value.ambient();
            _uniforms_to_set_v3[ diffuse_loc ] = value.diffuse();
            _uniforms_to_set_v3[ specular_loc ] = value.specular();
            _uniforms_to_set_f[ shininess_loc ] = value.shininess();

            GLuint i = 0;
            unsigned int diffuse_texs = 0;
            unsigned int specular_texs = 0;

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
        void uniform( const std::string& name, const ShaderLight& value ) {
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

    private:
        GLuint _program_id;

        mutable std::unordered_map<GLint, glm::mat4> _uniforms_to_set_m4;
        mutable std::unordered_map<GLint, glm::vec3> _uniforms_to_set_v3;
        mutable std::unordered_map<GLint, glm::vec4> _uniforms_to_set_v4;
        mutable std::unordered_map<GLint, GLfloat> _uniforms_to_set_f;
        mutable std::unordered_map<GLint, GLuint> _uniforms_to_set_u;
        mutable std::unordered_map<GLint, GLint> _uniforms_to_set_i;
        mutable std::unordered_map<GLenum, GLuint> _textures_to_set;

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
        std::shared_ptr<ShaderProgram> load( const std::vector<std::string>& vs_paths, const std::vector<std::string>& fs_paths ) const;
    private:
        std::string _load_file( const std::string& path ) const;
};

#endif // _REPLICATOR_SHADERS_HPP_


