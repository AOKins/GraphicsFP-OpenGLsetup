#version 450 core  

layout (location = 0) in vec4 position;

uniform mat4 perspective;
uniform mat4 projection;

out vec4 tex_cords;

void main(void) {
    tex_cords = position;

    //All modifications are pulled in via attributes
    //Optimization methods based from https://learnopengl.com/code_viewer_gh.php?code=src/4.advanced_opengl/6.1.cubemaps_skybox/6.1.skybox.vs
    vec4 iPos = perspective * projection * position;
    gl_Position = iPos.xyww;
}
