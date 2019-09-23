#version 330 core

layout (location = 0) in vec4 vertice_in;

void main() {
    gl_Position = vertice_in;
}
