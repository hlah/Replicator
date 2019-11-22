#include "camera.hpp"

#include "models.hpp"
#include "window.hpp"
#include "transform.hpp"

#include "spdlog/spdlog.h"

Camera::Camera( float fov, float near, float far, float aspect_ratio ) 
        : fov{fov}, near{near}, far{far}, aspect_ratio{aspect_ratio},
          projection_matrix{ matrix_op::perspective( fov, aspect_ratio, near, far ) } {}

void Camera::set_aspect_ratio( float aspect_ratio ) {
    this->aspect_ratio = aspect_ratio;
    projection_matrix = matrix_op::perspective( fov, aspect_ratio, near, far );
}

void camera_system( entt::registry& registry ) {
    auto current_camera_ptr = registry.try_ctx<CurrentCamera>();
    if( current_camera_ptr != nullptr ) {
        auto camera_ptr = registry.try_get<Camera>( current_camera_ptr->entity );
        if( camera_ptr != nullptr ) {
            auto window = registry.ctx<std::shared_ptr<Window>>();
            // Reset projection matrix on window resize
            if( window->resized() ) {
                camera_ptr->set_aspect_ratio( window->aspect_ratio() );
            }
            // Get view matrix
            glm::mat4 view_matrix{1.0};
            auto camera_transform_ptr = registry.try_get<Transform>( current_camera_ptr->entity );
            if( camera_transform_ptr != nullptr ) {
                view_matrix = glm::inverse( camera_transform_ptr->global_matrix() );
            }
            // TODO Optimization: update matrices only once for each shader program (from a model)
            auto& program_cache = registry.ctx<entt::resource_cache<ShaderProgram>>();
            program_cache.each([camera_ptr,&view_matrix](ShaderProgram& program){
                    program.uniform( "projection_transform", camera_ptr->projection_matrix );
                    program.uniform( "view_transform", view_matrix );
            });
        } 
        else {
            spdlog::warn("Camera entity with no Camera component!");
        }
    } else {
        spdlog::warn("No camera set!");
    }
}

