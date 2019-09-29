#ifndef _REPLICATOR_OBJECT_HPP_
#define _REPLICATOR_OBJECT_HPP_

#include "mesh.hpp"
#include "shaders.hpp"

#include "glad/glad.h"

#include <optional>

typedef size_t ObjectId;

class Object {
    public:
        // set object mesh
        void set_mesh( Mesh mesh ) { _mesh = mesh; }
        std::optional<Mesh>& get_mesh() { return _mesh; }
        const std::optional<Mesh>& get_mesh() const { return _mesh; };
        // add shader program
        void set_shader_program( const ShaderProgram& program ) { _shader_program = program; }
        std::optional<ShaderProgram>& get_shader_program() { return _shader_program; }
        const std::optional<ShaderProgram>& get_shader_program() const { return _shader_program; };
        // set model transform
        void set_model_transform( const glm::mat4& transform ) { _model_transform = transform; }
        std::optional<glm::mat4>& get_model_transform() { return _model_transform; }
        const std::optional<glm::mat4>& get_model_transform() const { return _model_transform; };
    private:
        std::vector<ObjectId> _children;
        std::optional<Mesh> _mesh;
        std::optional<ShaderProgram> _shader_program;
        std::optional<glm::mat4> _model_transform;
        ObjectId _parent;

        // private constructor (only World class can create objects)
        Object( ObjectId parent ) : _parent{parent} {};

        // add child object
        void add_child( const ObjectId child ) { _children.push_back(child); }

        friend class World;
};


#endif // _REPLICATOR_OBJECT_HPP_
