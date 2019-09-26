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
        void set_shader_program( const ShaderProgram program ) { _shader_program = program; }
    private:
        std::vector<ObjectId> _children;
        std::optional<Mesh> _mesh;
        std::optional<ShaderProgram> _shader_program;

        // add child object
        void add_child( const ObjectId child ) { _children.push_back(child); }

        friend class World;
};


#endif // _REPLICATOR_OBJECT_HPP_
