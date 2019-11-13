#include "deepcopy.hpp"

#include "hierarchy.hpp"

#include "spdlog/spdlog.h"

entt::entity deepcopy( entt::registry& registry, entt::entity entity, entt::entity copy_parent ) {

    auto entity_copy = registry.create( entity, registry, entt::exclude_t<Hierarchy>{} );

    // create new hierarchu for copy
    auto entity_hierarchy_ptr = registry.try_get<Hierarchy>( entity );
    if( entity_hierarchy_ptr != nullptr ) {
        registry.assign<Hierarchy>( entity_copy, copy_parent );
        auto child = entity_hierarchy_ptr->first();
        while( child != entt::null ) {
            deepcopy( registry, child, entity_copy );
            auto child_hierarchy = registry.get<Hierarchy>( child );
            child = child_hierarchy.next();
        }
    }

    return entity_copy;
}

void deepdelete( entt::registry& registry, entt::entity entity ) {
    // remove children
    auto entity_hierarchy_ptr = registry.try_get<Hierarchy>( entity );
    if( entity_hierarchy_ptr != nullptr ) {
        auto child = entity_hierarchy_ptr->first();
        while( child != entt::null ) {
            deepdelete( registry, child );
            child = entity_hierarchy_ptr->first();
        }
    }
    registry.destroy( entity );
}
