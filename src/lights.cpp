#include "lights.hpp"

#include "models.hpp"
#include "transform.hpp"

#include "glad/glad.h"

#include "spdlog/spdlog.h"

void light_system( entt::registry& registry ) {
    auto dir_light_view = registry.view<DirectionalLight, Transform>();
    
    GLuint texture_obj;
    glGenTextures(1, &texture_obj);
    glBindTexture(GL_TEXTURE_1D, texture_obj);

    auto model_view = registry.view<Model>();

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
} 
