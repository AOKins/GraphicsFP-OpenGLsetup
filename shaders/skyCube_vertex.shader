#version 450 core  

in vec4 cube_vertex;

uniform mat4 perspective;
uniform mat4 projection;

out vec4 tex_cords;

void main(void) {
    tex_cords = cube_vertex;

    //All modifications are pulled in via attributes    
    gl_Position = perspective * projection * cube_vertex;
}
