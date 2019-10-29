#ifndef _REPLICATOR_LIGHTS_H_
#define _REPLICATOR_LIGHTS_H_

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

#include "entt/entt.hpp"

class DirectionalLight {
    public:
        DirectionalLight( glm::vec3 color ) : _color{color} {}

        inline const glm::vec4& direction() const { return _direction; }
        inline const glm::vec3& color() const { return _color; }
    private:
        glm::vec3 _color;
        glm::vec4 _direction = glm::vec4{0.0, 0.0, -1.0, 0.0};

        friend void light_system( entt::registry& registry ); 
};


void light_system( entt::registry& registry ); 

#endif // _REPLICATOR_LIGHTS_H_
