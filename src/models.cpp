#include "models.hpp"

#include "transform.hpp"
#include "camera.hpp"

// model system
void model_system( entt::registry& registry ) {
    auto view = registry.view<Model, const Transform>();
    view.each([](auto entity, auto& model, const auto& transform){
            model.program->uniform( "model_transform", transform.global_matrix() );
            model.mesh.draw( *model.program );
    });
}
