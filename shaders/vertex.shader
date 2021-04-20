#version 450 core
// Taking in position
layout (location = 0) in vec4 position;

in vec2 obj_uv;
out vec2 vs_uv;

uniform mat4 toSpace;
uniform mat4 scale;
uniform mat4 camera;
uniform mat4 perspective;

void main() {
   gl_Position =  (perspective * camera * toSpace * scale * position) / position.w;
   vs_uv = obj_uv * vec2(1.0,-1.0);
}
