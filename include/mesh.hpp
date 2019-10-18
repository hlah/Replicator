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
        Mesh( 
            const std::vector<GLuint>& indices,
            const std::vector<glm::vec4>& vertices, 
            const std::vector<glm::vec4>& colors = {}
        );
        ~Mesh();

        // Draw mesh using program
        void draw( const ShaderProgram& program ) const;

    private:
        GLuint _index_buffer = 0;
        GLuint _vertex_buffer = 0;
        GLuint _color_buffer = 0;
        GLuint _vao = 0;
        size_t _index_array_size = 0;
        std::shared_ptr<bool> _ref_counter;
};

class MeshBuilder {
    public:
        // Add vertice to mesh
        void add_vertex( float x, float y, float z, float w=1.0 );
        // Add color attribute
        void add_color( float r, float g, float b, float a=1.0 );
        // Add index
        void add_index( GLuint index );

        // Add a cube with clockwise winding
        void cube( float side );

        // Build mesh
        Mesh build();

    private:
        std::vector<glm::vec4> _vertices;
        std::vector<glm::vec4> _colors;
        std::vector<GLuint> _indices;
};

class MeshCreationException : public std::exception {
    public:
        MeshCreationException( const std::string& msg ) : _msg{msg} {}
        const char* what() const throw() { return _msg.c_str(); }
    private:
        const std::string _msg;
};


#endif // _REPLICATOR_MESH_HPP_

