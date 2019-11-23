#include "hierarchy.hpp" 
#include "spdlog/spdlog.h"

// Flag resource for hierarchy update
struct HierarchyChanged{};

bool Hierarchy::compare( const entt::registry& registry, const entt::entity rhs ) const {
    if( rhs == entt::null || rhs == this->_parent || rhs == this->_prev ) {
        //spdlog::debug("\t1");
        return true;
    } else {
        if( this->_parent == entt::null ) {
            //spdlog::debug("\t2");
            return false;
        } else {
            auto& this_parent_h = registry.get<Hierarchy>( this->_parent );
            auto& rhs_h = registry.get<Hierarchy>( rhs );
            if( this_parent_h.compare( registry, rhs_h._parent ) ) {
                //spdlog::debug("\t3");
                return true;
            } 
        }
    }

    return false;
} 

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
                    prev_ent = current_hierarchy->_next;
                    current_hierarchy = registry.try_get<Hierarchy>( prev_ent );
                }
                // add new
                current_hierarchy->_next = entity;
                hierarchy._prev = prev_ent;
            }
        }
    }

    registry.set<HierarchyChanged>();
}


void Hierarchy::on_destroy(entt::entity entity, entt::registry& registry) {
    auto& hierarchy = registry.get<Hierarchy>( entity );
    // if is the first child
    if( hierarchy._prev == entt::null ) {
        if (hierarchy._parent != entt::null ) {
            auto parent_hierarchy = registry.try_get<Hierarchy>( hierarchy._parent );
            if( parent_hierarchy != nullptr ) {
                parent_hierarchy->_first = hierarchy._next;
                if( hierarchy._next != entt::null ) {
                    auto next_hierarchy = registry.try_get<Hierarchy>( hierarchy._next );
                    if( next_hierarchy != nullptr ) {
                        next_hierarchy->_prev = entt::null;
                    }
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

    registry.set<HierarchyChanged>();
}

void hierarchy_system( entt::registry& registry ) {
    // sort if any changes were made
    if( registry.try_ctx<HierarchyChanged>() != nullptr ) {
        registry.sort<Hierarchy>([&registry](const entt::entity lhs, const entt::entity rhs){
                auto& right_h = registry.get<Hierarchy>( rhs );
                return right_h.compare( registry, lhs );
        });
        registry.unset<HierarchyChanged>();
    }
}
