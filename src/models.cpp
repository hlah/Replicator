#include "models.hpp"

#include "transform.hpp"
#include "camera.hpp"
#include "material.hpp"

// model system
void model_system( entt::registry& registry ) {
    auto view = registry.view<Model, const Transform>();
    view.each([&registry](auto entity, auto& model, const auto& transform){
            model.program->uniform( "model_transform", transform.global_matrix() );
            auto material_ptr = registry.try_get<Material>( entity );
            if( material_ptr != nullptr ) {
                model.program->uniform( "material", *material_ptr );
            }
            model.mesh.draw( *model.program );
            if( material_ptr != nullptr ) {
                model.program->uniform( "material", Material{} );
            }
    });
}
