#ifndef _REPLICATOR_GEOMETRY_PLANE_H_
#define _REPLICATOR_GEOMETRY_PLANE_H_

#include "glm/vec3.hpp"

class Plane {
    public:
        Plane( const glm::vec3& position, const glm::vec3& normal ) :
            _position{position}, _normal{normal} {}

        inline const glm::vec3& position() const { return _position; }
        inline const glm::vec3& normal() const { return _normal; }
    private:
        glm::vec3 _position;
        glm::vec3 _normal;
};

#endif // _REPLICATOR_GEOMETRY_PLANE_H_
