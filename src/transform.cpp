#include "transform.hpp"
#include "hierarchy.hpp"

#include "spdlog/spdlog.h"
#include "matrix_op.hpp"

void transform_system( entt::registry& registry ) {
    auto group = registry.group<Transform, Hierarchy>();

    group.sort([&registry](const entt::entity lhs, const entt::entity rhs){
            auto& left_h = registry.get<Hierarchy>( lhs );
            auto& right_h = registry.get<Hierarchy>( rhs );
            return lhs == right_h.parent() || left_h.next() == rhs
                || (!(lhs == right_h.parent() || left_h.next() == rhs) 
                        && ( left_h.parent() < right_h.parent() || (left_h.parent() == right_h.parent() && &left_h < &right_h ) ) );
    });

    for( auto entity : group ) {
        auto& transform = registry.get<Transform>(entity);
        auto& hierarchy = registry.get<Hierarchy>(entity);
        if( hierarchy.parent() != entt::null ) {
            auto parent_transform_ptr = registry.try_get<Transform>(hierarchy.parent());
            if( parent_transform_ptr != nullptr ) {
                auto new_transform = transform;
                new_transform.global = parent_transform_ptr->global * transform.local;
                registry.replace<Transform>( entity, new_transform );
                continue;
            }
        }
        auto new_transform = transform;
        new_transform.global = transform.local;
        registry.replace<Transform>( entity, new_transform );
    }

}
