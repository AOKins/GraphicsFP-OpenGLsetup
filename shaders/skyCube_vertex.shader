#version 450 core  

in vec4 position;

uniform mat4 perspective;
uniform mat4 projection;

out vec4 tex_cords;

void main(void) {
    tex_cords = position;

    //All modifications are pulled in via attributes    
    gl_Position = perspective * projection * position;
}
