#version 450 core  

in vec4 cube_vertex;

uniform mat4 perspective;
uniform mat4 projection;

out vec4 texture_coordinates;

void main(void) {
    texture_coordinates = cube_vertex * -1;

    //All modifications are pulled in via attributes    
    gl_Position = perspective * projection * cube_vertex;
}
