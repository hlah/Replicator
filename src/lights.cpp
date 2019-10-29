#include "lights.hpp"

#include "models.hpp"
#include "transform.hpp"

#include "glad/glad.h"

#include "spdlog/spdlog.h"

void light_system( entt::registry& registry ) {
    auto dir_light_view = registry.view<DirectionalLight, Transform>();
    auto point_light_view = registry.view<PointLight, Transform>();
    
    GLuint texture_obj;
    glGenTextures(1, &texture_obj);
    glBindTexture(GL_TEXTURE_1D, texture_obj);

    auto model_view = registry.view<Model>();

    // Directional Lights
    size_t i = 0;
    dir_light_view.each([&i, &model_view](auto& dir_light, const auto& transform){
            std::string uniform_name 
                = std::string{"directional_lights["} 
                + std::to_string( i )
                + std::string{"]"};

                dir_light._direction = glm::normalize(transform.global_matrix() * glm::vec4{ 0.0, 0.0, -1.0, 0.0 });

            // TODO Optimization: update matrices only once for each shader program (from a model)
            model_view.each([&uniform_name, &dir_light](auto& model){
                    model.program->uniform( uniform_name, dir_light );
            });

            i++;
    });
    model_view.each([&dir_light_view](auto& model){
            model.program->uniform( "directional_lights_count", dir_light_view.size() );
    });

    // Point Lights
    i = 0;
    point_light_view.each([&i, &model_view](auto& point_light, const auto& transform){
            std::string uniform_name 
                = std::string{"point_lights["} 
                + std::to_string( i )
                + std::string{"]"};

            point_light._position = transform.global_matrix() * glm::vec4{ 0.0, 0.0, 0.0, 1.0 };

            // TODO Optimization: update matrices only once for each shader program (from a model)
            model_view.each([&uniform_name, &point_light](auto& model){
                    model.program->uniform( uniform_name, point_light );
            });

            i++;
    });
    model_view.each([&point_light_view](auto& model){
            model.program->uniform( "point_lights_count", point_light_view.size() );
    });

} 
