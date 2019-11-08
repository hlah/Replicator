#ifndef _REPLICATOR_MESH_HPP_
#define _REPLICATOR_MESH_HPP_

#define GLM_FORCE_SWIZZLE
#include "glm/vec4.hpp"
#include "glm/vec3.hpp"
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
            const std::vector<glm::vec4>& colors = {},
            const std::vector<glm::vec4>& normals = {},
            const std::vector<glm::vec2>& texcoords = {}
        );
        ~Mesh();

        // Draw mesh using program
        void draw( const ShaderProgram& program ) const;

    private:
        GLuint _index_buffer = 0;
        GLuint _vertex_buffer = 0;
        GLuint _color_buffer = 0;
        GLuint _normal_buffer = 0;
        GLuint _texcoord_buffer = 0;
        GLuint _vao = 0;
        size_t _index_array_size = 0;
        std::shared_ptr<bool> _ref_counter;
};

class MeshBuilder {
    public:
        // Add vertice to mesh
        void add_vertex( glm::vec3 v, unsigned int count=1 ) { add_vertex( glm::vec4{v, 1.0}, count ); };
        void add_vertex( glm::vec4 v, unsigned int count=1 );
        // Add color attribute
        void add_color( glm::vec3 c, unsigned int count=1 ) { add_color( glm::vec4{c, 1.0}, count ); };
        void add_color( glm::vec4 c, unsigned int count=1 );
        // Add normal
        void add_normal( glm::vec3 n, unsigned int count=1 ) { add_normal( glm::vec4{n, 0.0}, count ); };
        void add_normal( glm::vec4 n, unsigned int count=1 );
        // Add texture coordinates
        void add_texcoord( glm::vec2 t, unsigned int count=1 );
        // Add index
        void add_index( GLuint index );

        // Adde rectangle with normals
        void rect( glm::vec3 pos, glm::vec3 top, glm::vec3 right );
        // Add a cube with clockwise winding
        void cube( float side );
        // Add a circle with given radius and section
        void circle( glm::vec3 radius_angle, glm::vec3 front, unsigned int sections, glm::vec3 position = glm::vec3{0.0} );
        // Add a cylinder
        void cylinder( glm::vec3 radius_angle, glm::vec3 up_height, unsigned int sections, glm::vec3 position = glm::vec3{0.0} );

        // Build mesh
        Mesh build();

    private:
        std::vector<glm::vec4> _vertices;
        std::vector<glm::vec4> _colors;
        std::vector<glm::vec4> _normals;
        std::vector<glm::vec2> _texcoords;
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

