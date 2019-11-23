#ifndef _REPLICATOR_MATERIAL_H_
#define _REPLICATOR_MATERIAL_H_

#include "glm/vec3.hpp"

#include "entt/entt.hpp"

#include "texture.hpp"

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


        inline void set_ambient( const glm::vec3& value ) { _ambient = value; }
        inline void set_diffuse( const glm::vec3& value ) { _diffuse = value; }
        inline void set_specular( const glm::vec3& value ) { _specular = value; }

        void add_ambient_texture( entt::resource_handle<Texture> texture ) { _ambient_textures.push_back( texture ); }
        void add_diffuse_texture( entt::resource_handle<Texture> texture ) { _diffuse_textures.push_back( texture ); }
        void add_specular_texture( entt::resource_handle<Texture> texture ) { _specular_textures.push_back( texture ); }

        inline const glm::vec3& ambient() const { return _ambient; }
        inline const glm::vec3& diffuse() const { return _diffuse; }
        inline const glm::vec3& specular() const { return _specular; }
        inline float shininess() const { return _shininess; }
        inline const auto& ambient_textures() const { return _ambient_textures; }
        inline const auto& diffuse_textures() const { return _diffuse_textures; }
        inline const auto& specular_textures() const { return _specular_textures; }

    private:
        std::vector<entt::resource_handle<Texture>> _ambient_textures{};
        std::vector<entt::resource_handle<Texture>> _diffuse_textures{};
        std::vector<entt::resource_handle<Texture>> _specular_textures{};
        glm::vec3 _ambient;
        glm::vec3 _diffuse;
        glm::vec3 _specular;
        float _shininess;
};

#endif // _REPLICATOR_MATERIAL_H_
