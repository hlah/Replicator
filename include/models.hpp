#ifndef _REPLICATOR_MODEL_H_
#define _REPLICATOR_MODEL_H_

#include "shaders.hpp"
#include "mesh.hpp"

#include "entt/entt.hpp"

// Model compoenent
struct Model {
    Mesh mesh;
    entt::resource_handle<ShaderProgram> program;
};

// model system
void model_system( entt::registry& registry );

#endif // _REPLICATOR_MODEL_H_
