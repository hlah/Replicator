#ifndef _REPLICATOR_OBJECT_HPP_
#define _REPLICATOR_OBJECT_HPP_

#include "mesh.hpp"
#include "shaders.hpp"

#include "glad/glad.h"

#include <optional>

class Object {
    public:
        // set object mesh
        void set_mesh( Mesh mesh ) { _mesh = mesh; }
        // add shader program
        void set_shader_program( const ShaderProgram program ) { _shader_program = program; }
        // add child object
        void add_child( const Object& child ) { _children.push_back(child); }
        // draw object if is drawable (have mesh and shader program)
        void draw() const;
    private:
        std::vector<Object> _children;
        std::optional<Mesh> _mesh;
        std::optional<ShaderProgram> _shader_program;
};

#endif // _REPLICATOR_OBJECT_HPP_
