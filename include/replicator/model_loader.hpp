#ifndef _REPLICATOR_MODEL_LOADER_H_
#define _REPLICATOR_MODEL_LOADER_H_

#include "mesh.hpp"
#include "material.hpp"
#include "texture.hpp"
#include "transform.hpp"
#include "geometry/box.hpp"

#include "entt/entt.hpp"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <vector>

class ModelLoader {
    public:
        // Model loader constructor, revecie program handle to be attached to loaded meshes
        ModelLoader( entt::resource_handle<ShaderProgram> program_handle ) : _program_handle{program_handle} {
            _importer.SetPropertyInteger( AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_POINT | aiPrimitiveType_LINE );
        }

        // Load model into registry from file in given path.
        entt::entity load_model( 
                entt::registry& registry, 
                const std::string& path
        );

        // Get model bounding box.
        Box bounding_box();
    private:
        Assimp::Importer _importer;
        entt::resource_handle<ShaderProgram> _program_handle;
        std::vector<std::pair<Mesh, unsigned int>> _meshes;
        std::vector<MeshBuilder> _mesh_builders;
        std::vector<Material> _materials;

        void get_meshes( const aiScene* scene );
        void get_materials( entt::registry& registry, const aiScene* scene, const std::string directory );
        Transform get_transform( const aiNode* node ); 
        entt::resource_handle<Texture> get_texture( entt::registry& registry, const std::string path );

        entt::entity load_node( 
                entt::registry& registry, 
                aiNode* node,
                const entt::entity& parent = entt::null
        );

        Box _bounding_box( aiNode* node, const glm::mat4& transform );
};

class ModelLoaderException : public std::exception {
    public:
        ModelLoaderException( const std::string& msg ) : _msg{msg} { }
        const char* what() const throw() { return _msg.c_str(); }

    private:
        const std::string _msg;
};

#endif // _REPLICATOR_MODEL_LOADER_H_
