#ifndef _REPLICATOR_HIERARCHY_H_
#define _REPLICATOR_HIERARCHY_H_

#include "entt/entt.hpp"

class Hierarchy {
    public:
        Hierarchy( entt::entity p = entt::null ) : _parent{p} {}

        inline entt::entity parent() const { return _parent; }
        inline entt::entity next() const { return _next; }
        inline entt::entity prev() const { return _prev; }
        inline entt::entity first() const { return _first; }

        // Return true if rhs is an ancestor of rhs
        bool compare( const entt::registry& registry, const entt::entity rhs ) const;

        // update hierarchy components when hierarchy component is added
        static void on_construct(entt::entity entity, entt::registry& registry, Hierarchy& hierarchy);

        // update hierarchy components when hierarchy component is removed
        static void on_destroy(entt::entity entity, entt::registry& registry);

    private:
        entt::entity _parent = entt::null;
        entt::entity _first = entt::null;
        entt::entity _next = entt::null;
        entt::entity _prev = entt::null;
};


#endif // _REPLICATOR_HIERARCHY_H_

