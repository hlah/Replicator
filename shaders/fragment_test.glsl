#version 330 core

in vec4 color_f;
in vec4 normal_f;
in vec4 position_f;
out vec4 color;

uniform struct {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
} material;

struct directional_light {
    vec4 direction;
    vec3 color;
};

uniform mat4 view_transform;

uniform directional_light directional_lights[2];
uniform uint directional_lights_count;

vec3 ambient_light(vec3 light_color) {
    return material.ambient * light_color;
}

vec3 diffuse_light(vec3 light_color, vec4 light_direction) {
    return material.diffuse * light_color * max( dot(normal_f, light_direction), 0.0);
}

vec3 specular_light(vec3 light_color, vec4 light_direction, vec4 view_direction) {
    vec4 r = -light_direction + 2*normal_f*dot(normal_f, light_direction);
    return material.specular * light_color * max( pow( dot( r, view_direction ), material.shininess ), 0.0);
}

void main() {
    vec4 camera_pos = inverse(view_transform) * vec4(0.0, 0.0, 0.0, 1.0);
    vec4 view_direction = normalize( camera_pos - position_f );

    color = vec4(0.0, 0.0, 0.0, 1.0);

    // directional lights
    for( uint i=0u; i<directional_lights_count; i++ ) {
        color += vec4(ambient_light(directional_lights[i].color), 0.0);
        color += vec4(diffuse_light(directional_lights[i].color, -directional_lights[i].direction), 0.0);
        color += vec4(specular_light(directional_lights[i].color, -directional_lights[i].direction, view_direction), 0.0);
    }
}
