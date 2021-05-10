// Vertex Shader for shadow mapping
#version 450 core

layout (location = 0) in vec4 position;

uniform mat4 lightSpace;
uniform mat4 toSpace;

void main() {
    gl_Position = lightSpace * (toSpace * position) / position.w;
    gl_Position = gl_Position / gl_Position.w;
}
