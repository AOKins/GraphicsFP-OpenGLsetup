#version 450 core
// Taking in position, normal for that point, and UV
in vec4 position;
in vec4 normalVertex;
in vec2 obj_uv;

out vec4 vs_vertex;
out vec4 world_vertex;
out vec2 vs_uv;
out vec4 v_normal;

out vec4 lightPos;


uniform mat4 toSpace;
uniform mat4 scale;
uniform mat4 camera;
uniform mat4 perspective;


void main() {
    vs_uv = obj_uv * vec2(1.0,-1.0);

    mat4 toOri = toSpace / toSpace[3].w;
    toOri[3] = vec4(0,0,0,1); // Removing translation component
    
    v_normal = toOri * normalVertex;
    v_normal = normalize(v_normal / v_normal.w);

    lightPos = vec4(2,2,-2,1.0);

    vs_vertex = (toSpace * scale * position) / position.w;
    world_vertex = vs_vertex; 
    gl_Position = perspective * camera * world_vertex;
}
