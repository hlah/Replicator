#include "mesh.hpp"

Mesh::Mesh( 
        const std::vector<GLuint>& indices,
        const std::vector<glm::vec4>& vertices, 
        const std::vector<glm::vec4>& colors
) {
    // Check sizes
    if( colors.size() != 0 && colors.size() != vertices.size() ) {
        throw MeshCreationException{"Number of color attribues must be equal to vertices or zero!"};
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
        glDeleteBuffers(1, &_index_buffer);
    }
}

void Mesh::draw( const ShaderProgram program ) const {
    program.use();
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, _index_array_size, GL_UNSIGNED_INT, (GLvoid*)0);
    glBindVertexArray(0);
}


void MeshBuilder::add_vertex( float x, float y, float z, float w ) {
    _vertices.push_back( glm::vec4( x, y, z, w ) );
}

void MeshBuilder::add_color( float r, float g, float b, float a ) {
    _colors.push_back( glm::vec4( r, g, b, a ) );
}

void MeshBuilder::add_index( GLuint index ) {
    _indices.push_back( index );
}


void MeshBuilder::cube( float side ) {
    auto side_2 = side / 2.f;

    // vertices
    add_vertex( -side_2, side_2, side_2 );
    add_vertex( side_2, side_2, side_2 );
    add_vertex( side_2, -side_2, side_2 );
    add_vertex( -side_2, -side_2, side_2 );

    add_vertex( -side_2, side_2, -side_2 );
    add_vertex( side_2, side_2, -side_2 );
    add_vertex( side_2, -side_2, -side_2 );
    add_vertex( -side_2, -side_2, -side_2 );

    // indices
    //FRONT
    add_index(0); add_index(3); add_index(1);
    add_index(3); add_index(2); add_index(1);
    // BACK
    add_index(4); add_index(5); add_index(6);
    add_index(4); add_index(6); add_index(7);
    // LEFT
    add_index(0); add_index(4); add_index(7);
    add_index(3); add_index(0); add_index(7);
    // RIGHT
    add_index(5); add_index(1); add_index(2);
    add_index(6); add_index(5); add_index(2);
    // TOP
    add_index(4); add_index(0); add_index(5);
    add_index(0); add_index(1); add_index(5);
    // BOTTOM
    add_index(7); add_index(2); add_index(3);
    add_index(7); add_index(6); add_index(2);


}

Mesh MeshBuilder::build() {
    // if no indices are set use sequence of number of vertices TODO: should not modify _indices
    if( _vertices.size() > 0 && _indices.size() == 0 ) {
        for( unsigned int i=0; i<_vertices.size(); i++ ) {
            _indices.push_back(i);
        }
    }

    return Mesh{ _indices, _vertices, _colors };
}
