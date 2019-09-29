#include "world.hpp"

#include "spdlog/spdlog.h"

#include "matrix_op.hpp"

World::World() {
    // root is the parent of itself
    Object root{_next_id};
    _objects.emplace( std::make_pair(_next_id, root) ) ;
    _root_obj_id = _next_id++;
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
    return _root_obj_id;
}

void World::draw() const {
    // Get camera view matrix
    glm::mat4 view_transform{1.0};
    if( _camera_obj ) {
        auto current_obj_id = *_camera_obj;
        while( current_obj_id != _root_obj_id ) {
            auto& obj = _objects.at( current_obj_id );
            if( obj._model_transform ) {
                view_transform =  view_transform * (*obj._model_transform);
            }
            current_obj_id = obj._parent;
        }
        _warn_no_cam_set = true;
    } else if( _warn_no_cam_set ){
        spdlog::warn("No camera set!");
        _warn_no_cam_set = false;
    }
    view_transform = glm::inverse( view_transform );

    // Draw world tree
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
            /// TODO: optimize, should not set view matrix so often
            obj._shader_program->uniform( "view_transform", view_transform );
            obj._mesh->draw( *obj._shader_program );
        }

        for( auto child : obj._children ) {
            stack.push_back( child );
        }
    }
}

