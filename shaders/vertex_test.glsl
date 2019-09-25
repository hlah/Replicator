#version 330 core

layout (location = 0) in vec4 vertice_in;
layout (location = 1) in vec4 color_in;

out vec4 color_f;

void main() {
    gl_Position = vertice_in;
    color_f = color_in;
}
