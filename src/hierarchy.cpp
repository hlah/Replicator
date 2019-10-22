#include "hierarchy.hpp"

void Hierarchy::on_construct(entt::entity entity, entt::registry& registry, Hierarchy& hierarchy) {
    if( hierarchy._parent != entt::null  ) {
        auto parent_hierarchy = registry.try_get<Hierarchy>( hierarchy._parent );
        if( parent_hierarchy != nullptr ) {
            if( parent_hierarchy->_first == entt::null ) {
                parent_hierarchy->_first = entity;
            } else {
                // get last children
                auto prev_ent = parent_hierarchy->_first;
                auto current_hierarchy = registry.try_get<Hierarchy>( prev_ent );
                while( current_hierarchy != nullptr && current_hierarchy->_next != entt::null ) {
                    prev_ent = parent_hierarchy->_first;
                    current_hierarchy = registry.try_get<Hierarchy>( prev_ent );
                }
                // add new
                current_hierarchy->_next = entity;
                hierarchy._prev = prev_ent;
            }
        }
    }
}


void Hierarchy::on_destroy(entt::entity entity, entt::registry& registry) {
    auto& hierarchy = registry.get<Hierarchy>( entity );
    // if is the first child
    if( hierarchy._prev == entt::null ) {
        if (hierarchy._parent != entt::null ) {
            auto parent_hierarchy = registry.try_get<Hierarchy>( hierarchy._parent );
            if( parent_hierarchy != nullptr ) {
                parent_hierarchy->_first = hierarchy._next;
                auto next_hierarchy = registry.try_get<Hierarchy>( hierarchy._next );
                if( next_hierarchy != nullptr ) {
                    next_hierarchy->_prev = entt::null;
                }
            }
        } 
    } else {
        auto prev_hierarchy = registry.try_get<Hierarchy>( hierarchy._prev );
        if( prev_hierarchy != nullptr ) {
            prev_hierarchy->_next = hierarchy._next;
        }
        if( hierarchy._next != entt::null ) {
            auto next_hierarchy = registry.try_get<Hierarchy>( hierarchy._next );
            if( next_hierarchy != nullptr ){
                next_hierarchy->_prev = hierarchy._prev;
            }
        }
    }
}
