#version 450 core
// Taking in position, normal for that point, and UV
in vec4 position;
in vec4 normalVertex;
in vec2 obj_uv;

out vec4 vs_vertex;
out vec2 vs_uv;
out vec4 vs_normal;

uniform mat4 toSpace;
uniform mat4 scale;
uniform mat4 camera;
uniform mat4 perspective;

void main() {
    vs_vertex = (perspective * camera * toSpace * scale * position) / position.w;
    vs_uv = obj_uv * vec2(1.0,-1.0);
    vs_normal =  toSpace * normalVertex;
    gl_Position = vs_vertex;
}
