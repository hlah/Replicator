#ifndef _REPLICATOR_LIGHTS_H_
#define _REPLICATOR_LIGHTS_H_

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

#include "entt/entt.hpp"


struct ShaderLight {
    enum Type : unsigned int {
        None = 0,
        Directional = 1,
        Point = 2,
        Spotlight = 4,
    };
    glm::vec4 position{0.0, 0.0, 0.0, 0.0};
    glm::vec4 direction{0.0, 0.0, 0.0, 0.0};
    glm::vec3 color{0.0, 0.0, 0.0};

    float inner_angle = 0.0;
    float outer_angle = 0.0;

    Type type = None;
};

class LightColor {
    public:
        LightColor( glm::vec3 color ) : _color{color} {}
        inline const glm::vec3& color() const { return _color; }
    private:
        glm::vec3 _color;
};

class DirectionalLight{};
class PointLight {};
class Spotlight {
    public:
        Spotlight( float outer_angle )
            : _inner_angle{outer_angle},
              _outer_angle{outer_angle} {}
        Spotlight( float outer_angle, float inner_angle ) 
            : _inner_angle{inner_angle},
              _outer_angle{outer_angle} {}
        float inner() const { return _inner_angle; }
        float outer() const { return _outer_angle; }
    private:
        float _inner_angle; 
        float _outer_angle;
};

void light_system( entt::registry& registry ); 

#endif // _REPLICATOR_LIGHTS_H_
