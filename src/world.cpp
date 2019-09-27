#include "world.hpp"

#include "spdlog/spdlog.h"

World::World() {
    // root is the parent of itself
    Object root{_next_id};
    _objects.emplace( std::make_pair(_next_id, root) ) ;
    _root_id = _next_id++;
}

ObjectId World::create_object( ObjectId parent ) {
    Object obj{ parent };
    _objects.emplace( std::make_pair(_next_id, obj) ) ;
    get_object( parent ).add_child( _next_id );
    return _next_id++;
}

Object& World::get_object( ObjectId obj_id ) {
    return _objects.at( obj_id );
}

ObjectId World::root() const {
    return _root_id;
}

void World::draw() const {
    std::vector<ObjectId> stack;
    stack.push_back( root() );

    while( !stack.empty() ) {
        auto next = stack.back();
        stack.pop_back();


        auto obj = _objects.at( next );
        if( obj._mesh && obj._shader_program ) {
            if( obj._model_transform ) {
                obj._shader_program->uniform( "model_transform", *obj._model_transform );
            } else {
                obj._shader_program->uniform( "model_transform", glm::mat4{1.0} );
            }
            /// TODO: optimize, should not set projection matrix so often
            obj._shader_program->uniform( "proj_transform", _proj_matrix );
            obj._mesh->draw( *obj._shader_program );
        }

        for( auto child : obj._children ) {
            stack.push_back( child );
        }
    }
}

