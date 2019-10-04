#include "models.hpp"

#include "spdlog/spdlog.h"

// model system
void model_system( entt::registry& registry ) {
    auto view = registry.view<const Model>();
    view.each([](const auto& model){
            model.mesh.draw( model.program );
    });
}
