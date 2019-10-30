#include "lights.hpp"

#include "models.hpp"
#include "transform.hpp"

#include "glad/glad.h"

#include "spdlog/spdlog.h"

void light_system( entt::registry& registry ) {
    auto light_view = registry.view<LightColor, Transform>();
    auto model_view = registry.view<Model>();

    // Directional Lights
    size_t i = 0;
    light_view.each([&i, &model_view, &registry](const auto entity, const auto& light, const auto& transform){
            std::string uniform_name 
                = std::string{"lights["} 
                + std::to_string( i )
                + std::string{"]"};


            ShaderLight shader_light{};

            bool directional = registry.has<DirectionalLight>( entity ); 
            bool point = registry.has<PointLight>( entity ); 
            bool spotlight = registry.has<Spotlight>( entity ); 

            if( directional || spotlight ) {
                shader_light.direction = glm::normalize(transform.global_matrix() * glm::vec4{ 0.0, 0.0, -1.0, 0.0 });
            }
            if( point || spotlight ) {
                shader_light.position = transform.global_matrix() * glm::vec4{ 0.0, 0.0, 0.0, 1.0 };
            }

            if( directional ) {
                shader_light.type = (ShaderLight::Type)(shader_light.type | ShaderLight::Type::Directional);
            }
            if( point ) {
                shader_light.type = (ShaderLight::Type)(shader_light.type | ShaderLight::Type::Point);
            }
            if( spotlight ) {
                auto& spotlight_comp = registry.get<Spotlight>( entity );
                shader_light.type = (ShaderLight::Type)(shader_light.type | ShaderLight::Type::Spotlight);
                shader_light.inner_angle = spotlight_comp.inner();
                shader_light.outer_angle = spotlight_comp.outer();
            }

            shader_light.color = light.color();

            // TODO Optimization: update matrices only once for each shader program (from a model)
            model_view.each([&uniform_name, &shader_light](auto& model){
                    model.program->uniform( uniform_name, shader_light );
            });

            i++;
    });
    model_view.each([&light_view](auto& model){
            model.program->uniform( "light_count", light_view.size() );
    });
} 
