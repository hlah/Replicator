#version 330 core

#define DIRECTIONAL     1u
#define POINT           2u
#define SPOTLIGHT       4u

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

    for( uint i=0u; i<light_count; i++ ) {
        if( (lights[i].type & DIRECTIONAL) > 0u ) {
            vec4 light_direction = -lights[i].direction;
            color += vec4(ambient_light(lights[i].color), 0.0);
            color += vec4(diffuse_light(lights[i].color, light_direction), 0.0);
            color += vec4(specular_light(lights[i].color, light_direction, view_direction), 0.0);
        }
        if( (lights[i].type & POINT) > 0u ) {
            vec4 light_direction = normalize(lights[i].position - position_f);
            color += vec4(ambient_light(lights[i].color), 0.0);
            color += vec4(diffuse_light(lights[i].color, light_direction), 0.0);
            color += vec4(specular_light(lights[i].color, light_direction, view_direction), 0.0);
        }
        if( (lights[i].type & SPOTLIGHT) > 0u ) {
            vec4 light_direction = normalize(lights[i].position - position_f);
            float theta = dot(light_direction, normalize(-lights[i].direction));
            float intensity = clamp( (theta-lights[i].outer_angle) / (lights[i].inner_angle - lights[i].outer_angle), 0.0, 1.0); 
            color += vec4(ambient_light(lights[i].color), 0.0);
            color += intensity*vec4(diffuse_light(lights[i].color, light_direction), 0.0);
            color += intensity*vec4(specular_light(lights[i].color, light_direction, view_direction), 0.0);
            //color = vec4(theta, 0.0, 0.0, 1.0);
        }
    }

}
