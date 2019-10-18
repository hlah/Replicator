#include "camera.hpp"

#include "models.hpp"
#include "window.hpp"

#include "spdlog/spdlog.h"

Camera::Camera( float fov, float near, float far, float aspect_ratio ) 
        : fov{fov}, near{near}, far{far}, aspect_ratio{aspect_ratio},
          projection_matrix{ matrix_op::perspective( fov, aspect_ratio, near, far ) } {}

void Camera::set_aspect_ratio( float aspect_ratio ) {
    this->aspect_ratio = aspect_ratio;
    projection_matrix = matrix_op::perspective( fov, aspect_ratio, near, far );
}

void camera_system( entt::registry& registry ) {
    auto view = registry.view<Model>();
    auto camera_ptr = registry.try_ctx<Camera>();
    auto window = registry.ctx<std::shared_ptr<Window>>();
    if( camera_ptr != nullptr ) {
        if( window->resized() ) {
            camera_ptr->set_aspect_ratio( window->aspect_ratio() );
        }
        // TODO: update projectino matrix only once for each shader program (from a model)
        view.each([camera_ptr](auto& model){
                model.program->uniform( "projection_transform", camera_ptr->projection_matrix );
        });
    } else {
        view.each([](auto& model){
                spdlog::warn("No camera set!");
                model.program->uniform( "projection_transform", glm::mat4{1.0} );
        });
    }
}

