#ifndef _REPLICATOR_WORLD_HPP_
#define _REPLICATOR_WORLD_HPP_

#include "object.hpp"

#include <unordered_map>

class World {
    public:
        // Constructor
        World();

        // Create new object
        ObjectId create_object( ObjectId parent );
        // Get reference to object
        Object& get_object( ObjectId obj_id );
        // Return the root object id
        ObjectId root() const;

        // set projection matrix
        void set_projection_matrix( glm::mat4 proj ) { _proj_matrix = proj; }

        // Draw world
        void draw() const;
    private:
        ObjectId _root_id;
        ObjectId _next_id = 0;
        std::unordered_map<ObjectId, Object> _objects;

        glm::mat4 _proj_matrix;
};


#endif // _REPLICATOR_WORLD_HPP_
