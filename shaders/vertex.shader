// Fragment shader with attempt to simulate light sources
// Methodologies based on Scott Griffith's example
#version 450 core
// Taking in position, normal for that point, and UV
in vec4 position;
in vec4 normalVertex;
in vec2 obj_uv;

out vec4 vs_vertex;
out vec2 vs_uv;
out vec3 vs_normal;

out vec4 L_vertex;

uniform mat4 toSpace;
uniform mat4 camera;
uniform mat4 perspective;

uniform mat4 lightSpace;

uniform int LightCount;
uniform vec4 Lpos[32];

void main() {
    vs_uv = obj_uv * vec2(1.0,-1.0);

    mat4 toOri = toSpace;
    toOri[3] = vec4(0,0,0,1); // Removing translation component
    
    vec4 normal4d = toOri * normalVertex;
    normal4d = normal4d / normal4d.w;
    vs_normal = normalize(normal4d.xyz);

    vs_vertex = (toSpace * position) / position.w;

    L_vertex = lightSpace * vs_vertex;

    gl_Position = perspective * camera * vs_vertex; 
}
