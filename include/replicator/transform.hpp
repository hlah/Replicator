#ifndef _REPLICATOR_TRANSFORM_H_
#define _REPLICATOR_TRANSFORM_H_

#include "glm/vec4.hpp"
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
        Transform& rotate_global( const glm::quat& rot );

        inline void set_rotation( const glm::quat& rot ) { _rotation = rot; }
        const glm::quat& get_rotation() const { return _rotation; }

        // translation
        inline Transform& translate_global( float x, float y, float z ) { _translation += glm::vec4{x, y, z, 0.0}; return *this; }
        inline Transform& translate_global( const glm::vec3& translation ) { _translation += glm::vec4{translation, 0.0}; return *this; }
        inline Transform& translate_global( const glm::vec4& translation ) { _translation += translation; return *this; }
        inline Transform& translate( float x, float y, float z ) { return translate( glm::vec4{x, y, z, 0.0} ); }; 
        inline Transform& translate( const glm::vec3& translation ) { return translate( glm::vec4{translation, 0.0} ); }; 
        Transform& translate( const glm::vec4& translation );

        inline void set_translation( const glm::vec3& translation ) { _translation = glm::vec4{translation, 0.0}; }
        inline const glm::vec4& get_translation() const { return _translation; }

        // scale
        inline Transform& scale( float sx, float sy, float sz ) { _scale *= glm::vec3{sx, sy, sz}; return *this; }
        inline Transform& scale( float s ) { _scale *= glm::vec3{s, s, s}; return *this; }
        inline Transform& scale( const glm::vec3& s ) { _scale *= s; return *this; }

        inline void set_scale( const glm::vec3& s ) { _scale = s; }
        inline const glm::vec3& get_scale() const { return _scale; }

        // update transform components when transform component is added
        static void on_change(entt::entity entity, entt::registry& registry, Transform& transform);

    private:
        glm::vec4 _translation{0.0, 0.0, 0.0, 0.0};
        glm::vec3 _scale{1.0, 1.0, 1.0};
        glm::quat _rotation{1.0, 0.0, 0.0, 0.0};

        glm::mat4 _global{1.0};
        friend void transform_system( entt::registry& registry );
};


#endif // _REPLICATOR_TRANSFORM_H_
