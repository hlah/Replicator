#include "geometry/ray.hpp"

#include "camera.hpp"
#include "transform.hpp"
#include "window.hpp"

#include "spdlog/spdlog.h"

#include <limits>

std::optional<std::pair<float, glm::vec3>> Ray::intersects( const Plane& plane ) {
    std::optional<std::pair<float, glm::vec3>> result;

    auto denominator = glm::dot(_direction, plane.normal());
    if( denominator != 0.f ) {
        auto distance = glm::dot((plane.position() - _position), plane.normal())/denominator;
        auto point = _position + ( distance * _direction );
        result = {distance, point};
    }

    return result;
}

std::optional<std::pair<float, glm::vec3>> Ray::intersects( const Box& box ) {
    std::optional<std::pair<float, glm::vec3>> result;

    glm::vec3 candidates;
    for( size_t i=0; i<3; i++ ) {
        if( direction()[i] < 0.0 ) {
            candidates[i] = box.max()[i];
        } else {
            candidates[i] = box.min()[i];
        }
    }

    glm::vec3 distances{-std::numeric_limits<float>::infinity()};
    size_t max_i = 0;
    for( size_t i=0; i<3; i++ ) {
        if( direction()[i] != 0.0 ) {
            distances[i] = (candidates[i] - position()[i]) / direction()[i];
            if( distances[i] > distances[max_i] ) {
                max_i = i;
            }
        }
    }

    if( distances[max_i] >= 0.0 ) {
        size_t other1 = ( max_i + 1 ) % 3;
        size_t other2 = ( max_i + 2 ) % 3;
        auto point = position() + distances[max_i] * direction();
        if(     point[other1] <= box.max()[other1] 
             && point[other1] >= box.min()[other1] 
             && point[other2] <= box.max()[other2] 
             && point[other2] >= box.min()[other2] ) 
        {
            result = {distances[max_i], point};
        }
    }

    return result;
}

Ray Ray::from_screen( const entt::registry& registry, double screen_x, double screen_y ) {
    auto camera_entity = registry.ctx<CurrentCamera>().entity;
    auto camera = registry.get<Camera>( camera_entity );
    auto camera_transform = registry.get<Transform>( camera_entity );
    auto window = registry.ctx<WindowHandler>();
    auto pos_x = 2.0*screen_x/(float)window->width()-1.0;
    auto pos_y = 2.0*screen_y/(float)window->height()-1.0;

    glm::vec4 point_ndc{ pos_x, -pos_y, -1.0, 1.0 };
    auto point_view = glm::inverse( camera.projection_matrix ) * point_ndc;
    auto point_global = camera_transform.global_matrix() * point_view;
    point_global /= point_global.w;

    auto camera_pos = camera_transform.global_matrix() * glm::vec4{0.0, 0.0, 0.0, 1.0};
    auto point_dir = glm::normalize(point_global - camera_pos);

    return Ray{ camera_pos, point_dir };
}

