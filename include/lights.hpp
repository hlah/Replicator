#ifndef _REPLICATOR_LIGHTS_H_
#define _REPLICATOR_LIGHTS_H_

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

#include "entt/entt.hpp"

struct DirectionalLight {
    glm::vec4 direction;
    glm::vec3 color;
};


void light_system( entt::registry& registry ); 

#endif // _REPLICATOR_LIGHTS_H_
