#ifndef _REPLICATOR_RAY_H_
#define _REPLICATOR_RAY_H_

#include "geometry/plane.hpp"

#include "glm/vec3.hpp"
#include "glm/glm.hpp"

#include "entt/entt.hpp"

#include <optional>

class Ray {
    public:
        Ray( const glm::vec3& pos, const glm::vec3& dir ) :
            _position{pos}, _direction{dir} {}

        // checks intersection with plane
        std::optional<std::pair<float, glm::vec3>> intersects( const Plane& plane );

        inline const glm::vec3& position() const { return _position; }
        inline const glm::vec3& direction() const { return _direction; }

        static Ray from_screen( const entt::registry& registry, double screen_x, double screen_y );
    private:
        glm::vec3 _position;
        glm::vec3 _direction;
};

#endif // _REPLICATOR_RAY_H_
