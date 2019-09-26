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
        // add shader program
        void set_shader_program( const ShaderProgram& program ) { _shader_program = program; }
        // set model transform
        void set_model_transform( const glm::mat4& transform ) { _model_transform = transform; }
    private:
        std::vector<ObjectId> _children;
        std::optional<Mesh> _mesh;
        std::optional<ShaderProgram> _shader_program;
        std::optional<glm::mat4> _model_transform;

        // add child object
        void add_child( const ObjectId child ) { _children.push_back(child); }

        friend class World;
};


#endif // _REPLICATOR_OBJECT_HPP_
