#ifndef _REPLICATOR_MODEL_LOADER_H_
#define _REPLICATOR_MODEL_LOADER_H_

#include "mesh.hpp"
#include "material.hpp"
#include "texture.hpp"

#include "entt/entt.hpp"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <vector>

entt::entity load_model( 
        entt::registry& registry, 
        const std::string& path, 
        entt::resource_handle<ShaderProgram> program_handle 
);

std::vector<std::pair<Mesh,unsigned int>> get_meshes( const aiScene* scene );
std::vector<Material> get_materials( entt::registry& registry, const aiScene* scene, const std::string directory );
entt::resource_handle<Texture> get_texture( entt::registry& registry, const std::string path );
entt::entity load_node( 
        entt::registry& registry, 
        aiNode* node,
        const std::vector<std::pair<Mesh,unsigned int>>& meshes,
        const std::vector<Material>& materials,
        entt::resource_handle<ShaderProgram> program_handle,
        const entt::entity& parent = entt::null
);



class ModelLoadException : public std::exception {
    public:
        ModelLoadException( const std::string& msg ) : _msg{msg} { }
        const char* what() const throw() { return _msg.c_str(); }

    private:
        const std::string _msg;
};

#endif // _REPLICATOR_MODEL_LOADER_H_
