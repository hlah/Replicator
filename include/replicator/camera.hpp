#ifndef _REPLICATOR_CAMERA_H_
#define _REPLICATOR_CAMERA_H_

#include "glm/mat4x4.hpp"

#include "entt/entt.hpp"

#include "matrix_op.hpp"

struct CurrentCamera {
    entt::entity entity{entt::null};
};

struct Camera {
    float fov{M_PI/3}, near{-0.1}, far{-10.0}, aspect_ratio{1.0};
    glm::mat4 projection_matrix{1.0};

    // Create perspective camera
    Camera( float fov, float near, float far, float aspect_ratio=1.0 );


    // set aspect ration
    void set_aspect_ratio(float aspect_ratio);
};

void camera_system( entt::registry& registry );

#endif // _REPLICATOR_CAMERA_H_
