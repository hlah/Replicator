#include "mesh.hpp"

Mesh::Mesh( 
        const std::vector<GLuint>& indices,
        const std::vector<glm::vec4>& vertices, 
        const std::vector<glm::vec4>& colors,
        const std::vector<glm::vec4>& normals,
        const std::vector<glm::vec2>& texcoords
) {
    // Check sizes
    if( colors.size() != 0 && colors.size() != vertices.size() ) {
        throw MeshCreationException{"Number of color attributes must be equal to vertices or zero!"};
    }
    if( normals.size() != 0 && normals.size() != vertices.size() ) {
        throw MeshCreationException{"Number of normal attributes must be equal to vertices or zero!"};
    }
    if( texcoords.size() != 0 && texcoords.size() != vertices.size() ) {
        throw MeshCreationException{"Number of texture coordinate attributes must be equal to vertices or zero!"};
    }

    // Create vertex array object
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);


    // create vertex array
    size_t vertex_array_size = vertices.size()*4;
    // Create vertex buffer
    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertex_array_size, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*vertex_array_size, vertices.data());

    // Bind to Vao
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    _vertex_buffer = vertex_buffer;


    // create color array
    if( colors.size() > 0 ) {
        size_t color_array_size = colors.size()*4;
        // Create color buffer
        GLuint color_buffer;
        glGenBuffers(1, &color_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*color_array_size, NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*color_array_size, colors.data());

        // Bind to Vao
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        _color_buffer = color_buffer;
    }

    // create normal array
    if( normals.size() > 0 ) {
        size_t normal_array_size = normals.size()*4;
        // Create normal buffer
        GLuint normal_buffer;
        glGenBuffers(1, &normal_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*normal_array_size, NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*normal_array_size, normals.data());

        // Bind to Vao
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        _normal_buffer = normal_buffer;
    }

    // create texture coordinates array
    if( texcoords.size() > 0 ) {
        size_t texcoord_array_size = texcoords.size()*2;
        // Create texcoord buffer
        GLuint texcoord_buffer;
        glGenBuffers(1, &texcoord_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, texcoord_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*texcoord_array_size, NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*texcoord_array_size, texcoords.data());

        // Bind to Vao
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(3);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        _texcoord_buffer = texcoord_buffer;
    }


    // create index array
    _index_array_size = indices.size();

    // Create index buffer
    GLuint index_buffer;
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*_index_array_size, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLfloat)*_index_array_size, indices.data());


    glBindVertexArray(0);
    _index_buffer = vertex_buffer;

}

Mesh::~Mesh() {
    if( _ref_counter.unique() ) {
        glDeleteBuffers(1, &_vertex_buffer);
        glDeleteBuffers(1, &_normal_buffer);
        glDeleteBuffers(1, &_color_buffer);
        glDeleteBuffers(1, &_texcoord_buffer);
        glDeleteBuffers(1, &_index_buffer);
    }
}

void Mesh::draw( const ShaderProgram& program ) const {
    program.use();
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, _index_array_size, GL_UNSIGNED_INT, (GLvoid*)0);
    glBindVertexArray(0);
}


void MeshBuilder::add_vertex( glm::vec4 v, unsigned int count ) {
    for( unsigned int i=0; i<count; i++ ) {
        _vertices.push_back( v );
    }
}


void MeshBuilder::add_color( glm::vec4 c, unsigned int count ) {
    for( unsigned int i=0; i<count; i++ ) {
        _colors.push_back( c );
    }
}

void MeshBuilder::add_normal( glm::vec4 normal, unsigned int count ) {
    normal = glm::normalize( normal );
    for( unsigned int i=0; i<count; i++ ) {
        _normals.push_back( normal );
    }
}

void MeshBuilder::add_texcoord( glm::vec2 t, unsigned int count ) {
    for( unsigned int i=0; i<count; i++ ) {
        _texcoords.push_back( t );
    }
}



void MeshBuilder::add_index( GLuint index ) {
    _indices.push_back( index );
}

void MeshBuilder::rect( glm::vec3 pos, glm::vec3 top, glm::vec3 right ) {
    auto normal = glm::normalize( glm::cross(right, top) );
    add_vertex( pos - right + top );
    add_vertex( pos + right + top );
    add_vertex( pos + right - top );
    add_vertex( pos - right - top );

    add_texcoord( {0.0, 0.0} );
    add_texcoord( {1.0, 0.0} );
    add_texcoord( {1.0, 1.0} );
    add_texcoord( {0.0, 1.0} );

    add_normal( normal, 4 );

    add_index( _vertices.size()-4 );
    add_index( _vertices.size()-2 );
    add_index( _vertices.size()-3 );

    add_index( _vertices.size()-4 );
    add_index( _vertices.size()-1 );
    add_index( _vertices.size()-2 );
}

void MeshBuilder::cube( float side ) {
    auto side_2 = side / 2.f;
    // FRONT
    rect( {0.0, 0.0, side_2}, {0.0, side_2, 0.0}, {side_2, 0.0, 0.0} );
    // BACK
    rect( {0.0, 0.0, -side_2}, {0.0, side_2, 0.0}, {-side_2, 0.0, 0.0} );
    // LEFT
    rect( {-side_2, 0.0, 0.0}, {0.0, side_2, 0.0}, {0.0, 0.0, side_2} );
    // RIGHT
    rect( {side_2, 0.0, 0.0}, {0.0, side_2, 0.0}, {0.0, 0.0, -side_2} );
    // TOP
    rect( {0.0, side_2, 0.0}, {0.0, 0.0, -side_2}, {side_2, 0.0, 0.0} );
    // BOTTOM
    rect( {0.0, -side_2, 0.0}, {0.0, 0.0, side_2}, {side_2, 0.0, 0.0} );
}

Mesh MeshBuilder::build() {
    // if no indices are set use sequence of number of vertices TODO: should not modify _indices
    if( _vertices.size() > 0 && _indices.size() == 0 ) {
        for( unsigned int i=0; i<_vertices.size(); i++ ) {
            _indices.push_back(i);
        }
    }

    return Mesh{ _indices, _vertices, _colors, _normals, _texcoords };
}
