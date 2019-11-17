#include "mesh.hpp"

#include "matrix_op.hpp"

#include "spdlog/spdlog.h"


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

void MeshBuilder::cube( float side, const glm::vec3& pos ) {
    auto side_2 = side / 2.f;
    // FRONT
    rect( pos + glm::vec3{0.0, 0.0, side_2}, {0.0, side_2, 0.0}, {side_2, 0.0, 0.0} );
    // BACK
    rect( pos + glm::vec3{0.0, 0.0, -side_2}, {0.0, side_2, 0.0}, {-side_2, 0.0, 0.0} );
    // LEFT
    rect( pos + glm::vec3{-side_2, 0.0, 0.0}, {0.0, side_2, 0.0}, {0.0, 0.0, side_2} );
    // RIGHT
    rect( pos + glm::vec3{side_2, 0.0, 0.0}, {0.0, side_2, 0.0}, {0.0, 0.0, -side_2} );
    // TOP
    rect( pos + glm::vec3{0.0, side_2, 0.0}, {0.0, 0.0, -side_2}, {side_2, 0.0, 0.0} );
    // BOTTOM
    rect( pos + glm::vec3{0.0, -side_2, 0.0}, {0.0, 0.0, side_2}, {side_2, 0.0, 0.0} );
}

void MeshBuilder::circle( glm::vec3 radius_angle, glm::vec3 front, unsigned int sections, glm::vec3 position ) {
    auto prev_num_vertices = _vertices.size();
    // make radius_angle ortogonal to front
    radius_angle = radius_angle - (glm::dot( radius_angle, front ) * front);
    // central vertex
    add_vertex( position );
    // circunference vertices
    for( unsigned int i=0; i<sections; i++  ) {
        glm::vec4 dir = matrix_op::rotate( ((float)i/(float)sections)*M_PI*2.0, glm::vec4{front, 0.0} ) * glm::vec4{radius_angle, 0.0};
        add_vertex( position + dir.xyz() );
        //spdlog::debug("circle point: {} {} {}", dir.x, dir.y, dir.z);
    }
    add_normal( front, sections+1 );
    // add sections
    for( unsigned int i=0; i<sections; i++  ) {
        add_index( prev_num_vertices );
        add_index( prev_num_vertices+i+1 );
        add_index( prev_num_vertices+((i+1)%sections)+1 );
    }

}

void MeshBuilder::cylinder( glm::vec3 radius_angle, glm::vec3 up_height, unsigned int sections, glm::vec3 position ) {
    // top and bottom cicrles
    circle( radius_angle, up_height, sections, position+up_height );
    circle( radius_angle, -up_height, sections, position-up_height );

    // sides
    auto prev_num_vertices = _vertices.size();
    // vertices and normals
    for( unsigned int i=0; i<sections; i++ ) {
        glm::vec4 dir = matrix_op::rotate( ((float)i/(float)sections)*M_PI*2.0, glm::vec4{up_height, 0.0} ) * glm::vec4{radius_angle, 0.0};
        add_vertex( position + dir.xyz() + up_height );
        add_vertex( position + dir.xyz() - up_height );
        add_normal( dir, 2 );
    }
    // sections
    for( unsigned int i=0; i<sections; i++ ) {
        add_index( prev_num_vertices + (i*2)%(sections*2) );
        add_index( prev_num_vertices + (i*2+1)%(sections*2) );
        add_index( prev_num_vertices + (i*2+2)%(sections*2) );

        add_index( prev_num_vertices + (i*2+1)%(sections*2) );
        add_index( prev_num_vertices + (i*2+3)%(sections*2) );
        add_index( prev_num_vertices + (i*2+2)%(sections*2) );
    }

}

// Add icosphere
void MeshBuilder::icosphere( float radius, unsigned int divisions, glm::vec3 position ) {
    auto prev_num_vertices = _vertices.size();

    std::vector<glm::vec3> vertices;
    std::vector<glm::uvec3> triangles;

    auto g = (1.0f + sqrt(5.f)) / 2.f;

    vertices.push_back(glm::normalize(glm::vec3{-1, g, 0}));
    vertices.push_back(glm::normalize(normalize(glm::vec3{1, g, 0})));
    vertices.push_back(glm::normalize(normalize(glm::vec3{-1, -g, 0})));
    vertices.push_back(glm::normalize(normalize(glm::vec3{1, -g, 0})));
    vertices.push_back(glm::normalize(normalize(glm::vec3{0, -1, g})));
    vertices.push_back(glm::normalize(normalize(glm::vec3{0, 1, g})));
    vertices.push_back(glm::normalize(normalize(glm::vec3{0, -1, -g})));
    vertices.push_back(glm::normalize(normalize(glm::vec3{0, 1, -g})));
    vertices.push_back(glm::normalize(normalize(glm::vec3{g, 0, -1})));
    vertices.push_back(glm::normalize(normalize(glm::vec3{g, 0, 1})));
    vertices.push_back(glm::normalize(normalize(glm::vec3{-g, 0, -1})));
    vertices.push_back(glm::normalize(normalize(glm::vec3{-g, 0, 1})));

    triangles.push_back( {0, 11, 5} );
    triangles.push_back ( {0, 5 ,1 } );
    triangles.push_back( { 0 ,    1 ,    7} );
    triangles.push_back( { 0 ,    7 ,    10} );
    triangles.push_back( { 0 ,    10 ,   11} );
    triangles.push_back( { 1 ,    5 ,    9} );
    triangles.push_back( { 5 ,    11 ,   4} );
    triangles.push_back( { 11 ,   10 ,   2} );
    triangles.push_back( { 10 ,   7 ,    6} );
    triangles.push_back( { 7 ,    1 ,    8} );
    triangles.push_back( { 3 ,    9 ,    4} );
    triangles.push_back( { 3 ,    4 ,    2} );
    triangles.push_back( { 3 ,    2 ,    6} );
    triangles.push_back( { 3 ,    6 ,    8} );
    triangles.push_back( { 3 ,    8 ,    9} );
    triangles.push_back( { 4 ,    9 ,    5} );
    triangles.push_back( { 2 ,    4 ,    11} );
    triangles.push_back( { 6 ,    2 ,    10} );
    triangles.push_back( { 8 ,    6 ,    7} );
    triangles.push_back( { 9 ,    8 ,    1} );

    for( unsigned int d=0; d < divisions; d++ ) {
        std::vector<glm::uvec3> new_triangles;
        std::map<std::pair<unsigned int, unsigned int>, unsigned int> middle_vertices;

        for( const auto& triangle : triangles ) {
            auto mid_xy = _get_middle_vertex( middle_vertices, vertices, {triangle.x, triangle.y} );
            auto mid_yz = _get_middle_vertex( middle_vertices, vertices, {triangle.y, triangle.z} );
            auto mid_zx = _get_middle_vertex( middle_vertices, vertices, {triangle.z, triangle.x} );
            new_triangles.push_back( {triangle.x, mid_xy, mid_zx} );
            new_triangles.push_back( {triangle.y, mid_yz, mid_xy} );
            new_triangles.push_back( {triangle.z, mid_zx, mid_yz} );
            new_triangles.push_back( {mid_xy, mid_yz, mid_zx} );
        }

        triangles = std::move( new_triangles );
    }

    for( const auto& vertex : vertices ) {
        add_vertex( position + radius * vertex );
        add_normal( vertex );
    }

    for( const auto& triangle : triangles ) {
        add_index( prev_num_vertices + triangle.x );
        add_index( prev_num_vertices + triangle.y );
        add_index( prev_num_vertices + triangle.z );
    }

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

Box MeshBuilder::bounding_box( const glm::mat4& transform ) {
    glm::vec3 p1 = transform * _vertices[0];
    glm::vec3 p2 = transform * _vertices[0];
    for( size_t i=1; i<_vertices.size(); i++ ) {
        auto v = transform * _vertices[i];
        p1.x = std::min(p1.x, v.x);
        p1.y = std::min(p1.y, v.y);
        p1.z = std::min(p1.z, v.z);
        p2.x = std::max(p1.x, v.x);
        p2.y = std::max(p1.y, v.y);
        p2.z = std::max(p1.z, v.z);
    }
    return Box{ p1, p2 };
}

unsigned int MeshBuilder::_get_middle_vertex( std::map<std::pair<unsigned int, unsigned int>, unsigned int>& mp_map, std::vector<glm::vec3>& vertices, std::pair<unsigned int, unsigned int> segment ) {
    if( mp_map.count( segment ) == 1 ) {
        return mp_map.at(segment);
    }
    auto inverted = std::pair{ segment.second, segment.first };
    if( mp_map.count( inverted ) ) {
        return mp_map.at( inverted );
    } 
    auto middle_point = glm::normalize( vertices[ segment.first ] + vertices[ segment.second ] );
    vertices.push_back( middle_point );
    mp_map[ segment ] = vertices.size()-1;
    return vertices.size() - 1;
}
