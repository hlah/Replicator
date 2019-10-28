#include "lights.hpp"

#include "models.hpp"

#include "glad/glad.h"

void light_system( entt::registry& registry ) {
    auto dir_light_view = registry.view<DirectionalLight>();
    
    GLuint texture_obj;
    glGenTextures(1, &texture_obj);
    glBindTexture(GL_TEXTURE_1D, texture_obj);

    auto model_view = registry.view<Model>();

    size_t i = 0;
    dir_light_view.each([&i, &model_view](const auto& dir_light){
            std::string uniform_name 
                = std::string{"directional_lights["} 
                + std::to_string( i )
                + std::string{"]"};

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
