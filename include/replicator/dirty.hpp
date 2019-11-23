#ifndef _REPLICATOR_DIRTY_H_
#define _REPLICATOR_DIRTY_H_

#include "entt/entt.hpp"

#include "spdlog/spdlog.h"

template <class T>
class Dirty {
    public:
        static void dirty_recurse( entt::registry& registry, entt::entity entity ) {
            if( !registry.has<Dirty<T>>( entity ) ) {
                if( registry.has<T>( entity ) ) {
                    registry.assign<Dirty<T>>( entity );
                }
                auto hierarchy_ptr = registry.try_get<Hierarchy>( entity );
                if( hierarchy_ptr != nullptr ) {
                    auto next_child = hierarchy_ptr->first();
                    while( next_child != entt::null ) {
                        dirty_recurse( registry, next_child );
                        next_child = registry.get<Hierarchy>( next_child ).next();
                    }
                }
            }
        }
};

#endif // _REPLICATOR_DIRTY_H_
