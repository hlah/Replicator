#include "transform.hpp"

void transform_system( entt::registry& registry ) {
    auto view = registry.view<Transform>();
    view.each([](auto& transform){
            transform.global= transform.local;
    });
}
