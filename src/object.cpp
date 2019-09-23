#include "object.hpp"

void Object::draw() const {
    for( const auto& child : _children ) {
        child.draw();
    }

    if( _mesh && _shader_program ) {
        _mesh->draw( *_shader_program );
    }
}

