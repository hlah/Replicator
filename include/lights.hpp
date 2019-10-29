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
        SpotLight = 4,
    };
    glm::vec4 position{0.0, 0.0, 0.0, 0.0};
    glm::vec4 direction{0.0, 0.0, 0.0, 0.0};
    glm::vec3 color{0.0, 0.0, 0.0};
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

void light_system( entt::registry& registry ); 

#endif // _REPLICATOR_LIGHTS_H_
