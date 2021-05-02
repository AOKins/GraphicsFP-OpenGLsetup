// Fragment shader with attempt to simulate light sources
// Methodologies based on Scott Griffith's example
#version 450 core
// Taking in position, normal for that point, and UV
in vec4 position;
in vec4 normalVertex;
in vec2 obj_uv;

out vec4 vs_vertex;
out vec4 cameraS_vertex;
out vec2 vs_uv;
out vec3 vs_normal;

out vec4 lightPos;
out vec3 lightColor;
out float lightIntensity;

uniform mat4 toSpace;
uniform mat4 camera;
uniform mat4 perspective;

void main() {
    vs_uv = obj_uv * vec2(1.0,-1.0);

    mat4 toOri = toSpace;
    toOri[3] = vec4(0,0,0,1); // Removing translation component
    
    vec4 normal4d = toOri * normalVertex;
    normal4d = normal4d / normal4d.w;
    vs_normal = normalize(normal4d.xyz);

    vs_vertex = (toSpace * position) / position.w;
    cameraS_vertex = perspective * camera * vs_vertex; 
    gl_Position = cameraS_vertex;
}
