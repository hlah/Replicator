#ifndef _REPLICATOR_TRANSFORM_H_
#define _REPLICATOR_TRANSFORM_H_

#include "glm/mat4x4.hpp"

#include "entt/entt.hpp"

struct Transform {
    glm::mat4 local{1.0};
    glm::mat4 global{1.0};
};

void transform_system( entt::registry& registry );

#endif // _REPLICATOR_TRANSFORM_H_
