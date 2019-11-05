#ifndef _REPLICATOR_TRANSFORM_H_
#define _REPLICATOR_TRANSFORM_H_

#include "glm/mat4x4.hpp"
#include "glm/gtx/quaternion.hpp"

#include "entt/entt.hpp"

void transform_system( entt::registry& registry );

class Transform {
    public:
        // Create identity transform;
        Transform() {}

        // get transform local matrix
        glm::mat4 local_matrix() const;

        // get global transformation matrix
        inline const glm::mat4& global_matrix() const { return _global; }

        // rotations
        Transform& rotate_x( float angle );
        Transform& rotate_y( float angle );
        Transform& rotate_z( float angle );
        Transform& rotate( const glm::quat& rot );
        Transform& rotate_x_global( float angle );
        Transform& rotate_y_global( float angle );
        Transform& rotate_z_global( float angle );

        // translation
        inline Transform& translate_global( float x, float y, float z ) { _translation += glm::vec4{x, y, z, 0.0}; return *this; }
        Transform& translate( float x, float y, float z ); 

        // scale
        inline Transform& scale( float sx, float sy, float sz ) { _scale *= glm::vec3{sx, sy, sz}; return *this; }

    private:
        glm::vec4 _translation{0.0, 0.0, 0.0, 0.0};
        glm::vec3 _scale{1.0, 1.0, 1.0};
        glm::quat _rotation{1.0, 0.0, 0.0, 0.0};

        glm::mat4 _global{1.0};
        friend void transform_system( entt::registry& registry );
};


#endif // _REPLICATOR_TRANSFORM_H_
