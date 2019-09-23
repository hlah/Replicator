#ifndef _REPLICATOR_MESH_HPP_
#define _REPLICATOR_MESH_HPP_

#include <glm/vec4.hpp>
#include "glad/glad.h"

#include <vector>
#include <memory>

#include "shaders.hpp"

class Mesh {
    public:
        // Create mesh from vertice list and index list
        Mesh(  const std::vector<glm::vec4>& vertices, const std::vector<GLuint>& indices );
        ~Mesh();

        // Draw mesh using program
        void draw( const ShaderProgram progrma ) const;

    private:
        GLuint _vertex_buffer;
        GLuint _index_buffer;
        GLuint _vao = 0;
        size_t _index_array_size = 0;
        std::shared_ptr<bool> _ref_counter;
};

class MeshBuilder {
    public:
        // Add vertice to mesh
        void add_vertex( float x, float y, float z );

        // Build mesh
        Mesh build();

    private:
        std::vector<glm::vec4> _vertices;
        std::vector<GLuint> _indices;
};



#endif // _REPLICATOR_MESH_HPP_

