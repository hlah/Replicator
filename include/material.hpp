#ifndef _REPLICATOR_MATERIAL_H_
#define _REPLICATOR_MATERIAL_H_

#include "glm/vec3.hpp"

#include "entt/entt.hpp"

class Material {
    public:
        Material( 
            glm::vec3 ambient = glm::vec3{0.0},
            glm::vec3 diffuse = glm::vec3{0.0},
            glm::vec3 specular = glm::vec3{0.0},
            float shininess = 0.0
        ) : _ambient{ambient}, _diffuse{diffuse}, _specular{specular}, _shininess{shininess} {}

        Material(
            glm::vec3 color,
            float ambient,
            float diffuse,
            float specular,
            float shininess
        ) : _ambient{color*ambient}, _diffuse{color*diffuse}, _specular{color*specular}, _shininess{shininess} {}

        inline const glm::vec3& ambient() const { return _ambient; }
        inline const glm::vec3& diffuse() const { return _diffuse; }
        inline const glm::vec3& specular() const { return _specular; }
        inline float shininess() const { return _shininess; }

    private:
        glm::vec3 _ambient;
        glm::vec3 _diffuse;
        glm::vec3 _specular;
        float _shininess;
};

#endif // _REPLICATOR_MATERIAL_H_
