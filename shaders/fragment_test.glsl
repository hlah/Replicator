#version 330 core

#define DIRECTIONAL     1u
#define POINT           2u
#define SPOTLIGHT       4u

in vec4 color_f;
in vec4 normal_f;
in vec4 position_f;
in vec2 texcoords_f;
out vec4 color;

uniform sampler2D texture1;

uniform struct {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;

    sampler2D diffuse_textures[4];
    uint diffuse_texture_count;
    sampler2D specular_textures[4];
    uint specular_texture_count;
} material;

struct light {
    uint type;

    vec4 position;
    vec4 direction;

    vec3 color;

    float outer_angle;
    float inner_angle;
};

uniform mat4 view_transform;

uniform light lights[16];
uniform uint light_count;

vec3 ambient_light(vec3 obj_color, vec3 light_color) {
    return obj_color * light_color;
}

vec3 diffuse_light(vec3 obj_color, vec3 light_color, vec4 light_direction) {
    return obj_color * light_color * max( dot(normal_f, light_direction), 0.0);
}

vec3 specular_light(vec3 obj_color, vec3 light_color, vec4 light_direction, vec4 view_direction) {
    vec4 r = normalize(-light_direction + 2*normal_f*dot(normal_f, light_direction));
    return obj_color * light_color * max( pow( dot( r, view_direction ), material.shininess ), 0.0);
}

vec4 apply_lights( vec3 ambient_color, vec3 diffuse_color, vec3 specular_color ) {
    vec4 color = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 camera_pos = inverse(view_transform) * vec4(0.0, 0.0, 0.0, 1.0);
    vec4 view_direction = normalize( camera_pos - position_f );
    for( uint i=0u; i<light_count; i++ ) {
        if( (lights[i].type & DIRECTIONAL) > 0u ) {
            vec4 light_direction = -lights[i].direction;
            color += vec4(ambient_light(ambient_color, lights[i].color), 0.0);
            color += vec4(diffuse_light(diffuse_color, lights[i].color, light_direction), 0.0);
            color += vec4(specular_light(specular_color, lights[i].color, light_direction, view_direction), 0.0);
        }
        if( (lights[i].type & POINT) > 0u ) {
            vec4 light_direction = normalize(lights[i].position - position_f);
            color += vec4(ambient_light(ambient_color, lights[i].color), 0.0);
            color += vec4(diffuse_light(diffuse_color, lights[i].color, light_direction), 0.0);
            color += vec4(specular_light(specular_color, lights[i].color, light_direction, view_direction), 0.0);
        }
        if( (lights[i].type & SPOTLIGHT) > 0u ) {
            vec4 light_direction = normalize(lights[i].position - position_f);
            float theta = dot(light_direction, normalize(-lights[i].direction));
            float intensity = clamp( (theta-lights[i].outer_angle) / (lights[i].inner_angle - lights[i].outer_angle), 0.0, 1.0); 
            color += vec4(ambient_light(ambient_color, lights[i].color), 0.0);
            color += intensity*vec4(diffuse_light(diffuse_color, lights[i].color, light_direction), 0.0);
            color += intensity*vec4(specular_light(specular_color, lights[i].color, light_direction, view_direction), 0.0);
        }
    }
    return color;
}

void main() {

    vec3 ambient_color = material.ambient;
    vec3 diffuse_color = material.diffuse;
    vec3 specular_color = material.specular;

    for( uint i=0u; i<material.diffuse_texture_count; i++ ) {
        vec3 tex_color = texture( material.diffuse_textures[i], texcoords_f ).rgb;
        //ambient_color += tex_color;
        diffuse_color += tex_color;
    }
    for( uint i=0u; i<material.specular_texture_count; i++ ) {
        vec3 tex_color = texture( material.specular_textures[i], texcoords_f ).rgb;
        specular_color += tex_color;
    }

    //diffuse_color = vec3(0.0);
    //ambient_color = vec3(0.0);
    color = apply_lights( ambient_color, diffuse_color, specular_color );
    //color = vec4((normal_f.xyz+1.0)/2.0, 1.0);
}
