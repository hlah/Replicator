#version 330 core

layout (location = 0) in vec4 vertice_in;
layout (location = 1) in vec4 color_in;

out vec4 color_f;

uniform mat4 model_transform;

void main() {
    gl_Position = model_transform * vertice_in;
    color_f = color_in;
}
