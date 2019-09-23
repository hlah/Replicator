#include "mesh.hpp"

Mesh::Mesh(  const std::vector<glm::vec4>& vertices, const std::vector<GLuint>& indices ) {
    // Create vertex array object
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    // create vertex array
    size_t vertex_array_size = vertices.size()*4;
    auto vertex_array = new GLfloat[vertex_array_size];
    size_t i = 0;
    for( const auto& vertex : vertices ) {
        vertex_array[i++] = vertex[0];
        vertex_array[i++] = vertex[1];
        vertex_array[i++] = vertex[2];
        vertex_array[i++] = vertex[3];
    }
    // Create vertex buffer
    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertex_array_size, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*vertex_array_size, vertex_array);

    // Bind to Vao
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // create index array
    _index_array_size = vertices.size();
    auto index_array = new GLuint[_index_array_size];
    i = 0;
    for( const auto& index : indices ) {
        index_array[i++] = index;
    }

    // Create index buffer
    GLuint index_buffer;
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*_index_array_size, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLfloat)*_index_array_size, index_array);


    glBindVertexArray(0);

    delete[] vertex_array;
    delete[] index_array;

    _vertex_buffer = vertex_buffer;
    _vertex_buffer = vertex_buffer;
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


void MeshBuilder::add_vertex( float x, float y, float z ) {
    _vertices.push_back( glm::vec4( x, y, z, 1.0 ) );
}

Mesh MeshBuilder::build() {
    // if no indices are set use sequence of number of vertices TODO: should not modify _indices
    if( _vertices.size() > 0 && _indices.size() == 0 ) {
        for( unsigned int i=0; i<_vertices.size(); i++ ) {
            _indices.push_back(i);
        }
    }

    return Mesh{ _vertices, _indices };
}
