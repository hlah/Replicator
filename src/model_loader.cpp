#include "model_loader.hpp"

#include "hierarchy.hpp"
#include "transform.hpp"
#include "models.hpp"

#include "spdlog/spdlog.h"

entt::entity load_model( 
        entt::registry& registry, 
        const std::string& path, 
        entt::resource_handle<ShaderProgram> program_handle 
) {
    Assimp::Importer importer;

    const auto scene = importer.ReadFile(
            path,
            aiProcess_Triangulate | aiProcess_GenNormals
    );

    if( !scene || !scene->mRootNode ) {
        throw ModelLoadException{ importer.GetErrorString() };
    }

    spdlog::debug("Loaded '{}''", path);

    std::string directory = path.substr(0, path.find_last_of('/'));

    auto meshes = get_meshes( scene );
    auto materials = get_materials( registry, scene, directory );

    auto model_root = load_node( registry, scene->mRootNode, meshes, materials, program_handle );
    return model_root;
}

std::vector<std::pair<Mesh,unsigned int>> get_meshes( const aiScene* scene ) {
    std::vector<std::pair<Mesh,unsigned int>> meshes;

    for(unsigned int i=0; i<scene->mNumMeshes; i++) {
        const auto assimp_mesh = scene->mMeshes[i];

        MeshBuilder mb;
        std::string mesh_attributes;

        // get positions
        if( assimp_mesh->HasPositions() ) {
            for( unsigned int j=0; j<assimp_mesh->mNumVertices; j++ ) {
                mb.add_vertex( glm::vec3{
                        assimp_mesh->mVertices[j].x,
                        assimp_mesh->mVertices[j].y,
                        assimp_mesh->mVertices[j].z,
                } );
            }
            mesh_attributes += std::string{"Positions "};
        }

        // get colors
        if( assimp_mesh->mColors[0] != nullptr ) {
            for( unsigned int j=0; j<assimp_mesh->mNumVertices; j++ ) {
                mb.add_color( glm::vec4{
                        assimp_mesh->mColors[0][j].r,
                        assimp_mesh->mColors[0][j].g,
                        assimp_mesh->mColors[0][j].b,
                        assimp_mesh->mColors[0][j].a
                } );
            }
            mesh_attributes += std::string{"Colors "};
        }

        // get normals
        if( assimp_mesh->HasNormals() ) {
            for( unsigned int j=0; j<assimp_mesh->mNumVertices; j++ ) {
                mb.add_normal( glm::vec3{
                        assimp_mesh->mNormals[j].x,
                        assimp_mesh->mNormals[j].y,
                        assimp_mesh->mNormals[j].z,
                } );
            }
            mesh_attributes += std::string{"Normals "};
        }

        // get texture coordinates
        if( assimp_mesh->mTextureCoords[0] != nullptr) {
            for( unsigned int j=0; j<assimp_mesh->mNumVertices; j++ ) {
                mb.add_texcoord( glm::vec2{
                        assimp_mesh->mTextureCoords[0][j].x,
                        assimp_mesh->mTextureCoords[0][j].y,
                } );
            }
            mesh_attributes += std::string{"TextureCoords "};
        }

        // get indices
        for( unsigned int j=0; j<assimp_mesh->mNumFaces; j++ ) {
            const auto face = assimp_mesh->mFaces[j];
            for( unsigned int k=0; k<face.mNumIndices; k++ ) {
                mb.add_index( face.mIndices[k] );
            }
        }

        spdlog::debug("Mesh '{}' ({}): {}; {} verties; {} triangles", (assimp_mesh->mName).C_Str(), i, mesh_attributes, assimp_mesh->mNumVertices, assimp_mesh->mNumFaces);

        auto mesh_material_index = assimp_mesh->mMaterialIndex;

        meshes.emplace_back( mb.build(), mesh_material_index );
    }
    return meshes;
}

std::vector<Material> get_materials( entt::registry& registry, const aiScene* scene, const std::string directory ) {
    std::vector<Material> materials;

    for( unsigned int i=0; i<scene->mNumMaterials; i++ ) {
        const auto assimp_material = scene->mMaterials[i];

        aiColor3D ambient_color;
        aiColor3D diffuse_color;
        aiColor3D specular_color;
        aiString name;
        float shininess;

        assimp_material->Get( AI_MATKEY_COLOR_AMBIENT, ambient_color );
        assimp_material->Get( AI_MATKEY_COLOR_DIFFUSE, diffuse_color );
        assimp_material->Get( AI_MATKEY_COLOR_SPECULAR, specular_color );
        assimp_material->Get( AI_MATKEY_SHININESS, shininess );
        assimp_material->Get( AI_MATKEY_NAME, name );

        materials.emplace_back(
                glm::vec3{ ambient_color.r, ambient_color.g, ambient_color.b },
                glm::vec3{ diffuse_color.r, diffuse_color.g, diffuse_color.b },
                glm::vec3{ specular_color.r, specular_color.g, specular_color.b },
                shininess
        );

        spdlog::debug("Material '{}' ({}):", name.C_Str(), i);
        spdlog::debug("Ambient: r: {}, g: {}, b: {}", ambient_color.r, ambient_color.g, ambient_color.b);
        spdlog::debug("Diffuse: r: {}, g: {}, b: {}", diffuse_color.r, diffuse_color.g, diffuse_color.b);
        spdlog::debug("Specular: r: {}, g: {}, b: {}", specular_color.r, specular_color.g, specular_color.b);
        spdlog::debug("Shininess: {}", shininess);

        // Diffuse textures:
        for( unsigned int j=0; j<assimp_material->GetTextureCount(aiTextureType_DIFFUSE); j++ ) {
            aiString texture_path;
            assimp_material->GetTexture(aiTextureType_DIFFUSE, j, &texture_path);
            spdlog::debug("Diffuse texture: '{}'", texture_path.C_Str());
            auto texture_handle = get_texture( registry, directory + std::string{"/"} + std::string{texture_path.C_Str()} );
            if( texture_handle ) {
                materials.back().add_diffuse_texture( texture_handle );
            }
        }

        // Specular
        for( unsigned int j=0; j<assimp_material->GetTextureCount(aiTextureType_SPECULAR); j++ ) {
            aiString texture_path;
            assimp_material->GetTexture(aiTextureType_SPECULAR, j, &texture_path);
            spdlog::debug("Specular texture: '{}'", texture_path.C_Str());
            auto texture_handle = get_texture( registry, directory + std::string{"/"} + std::string{texture_path.C_Str()} );
            if( texture_handle ) {
                materials.back().add_specular_texture( texture_handle );
            }
        }
    }
    return materials;
}

entt::resource_handle<Texture> get_texture( entt::registry& registry, const std::string path ) {
    try {
        auto& texture_cache = registry.ctx<entt::resource_cache<Texture>>();
        auto texture_handle = texture_cache.load<TextureLoader>( entt::hashed_string{path.c_str()}, path );
        return texture_handle;
    } catch( TextureException& e ) {
        spdlog::error("Could not load texture '{}': ", path, e.what());
        return entt::resource_handle<Texture>{};
    }
}

entt::entity load_node( 
        entt::registry& registry, 
        aiNode* node,
        const std::vector<std::pair<Mesh,unsigned int>>& meshes,
        const std::vector<Material>& materials,
        entt::resource_handle<ShaderProgram> program_handle,
        const entt::entity& parent
        ) {
    auto node_entity = registry.create();

    // add transform
    aiVector3D scalling;
    aiQuaternion rotation;
    aiVector3D translation;
    node->mTransformation.Decompose( scalling, rotation, translation );

    Transform transform;
    transform.scale( scalling.x, scalling.y, scalling.z );
    transform.rotate( glm::quat{ rotation.w, rotation.x, rotation.y, rotation.z } );
    transform.translate( translation.x, translation.y, translation.z );
    registry.assign<Transform>( node_entity, transform );

    // add parent
    registry.assign<Hierarchy>( node_entity, parent );

    // add meshes
    for( unsigned int i=0; i<node->mNumMeshes; i++ ) {
        auto mesh_entity = registry.create();
        registry.assign<Model>( mesh_entity, meshes[node->mMeshes[i]].first, program_handle );
        registry.assign<Material>( mesh_entity, materials[ meshes[node->mMeshes[i]].second  ] );
        registry.assign<Transform>( mesh_entity );
        registry.assign<Hierarchy>( mesh_entity, node_entity );
    }

    // add children
    for( unsigned int i=0; i<node->mNumChildren; i++ ) {
        load_node( registry, node->mChildren[i], meshes, materials, program_handle, node_entity );
    }

    return node_entity;
}
