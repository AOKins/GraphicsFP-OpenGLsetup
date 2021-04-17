#version 450 core                                                 

in vec4 texture_coordinates;
uniform samplerCube cube_texture;
out vec4 color;                                                   

void main(void) {
    // Use cube_texture to interpolate the correct texture color
    color = vec4(1.0f,1.0f,1.0f,1.0f);//texture(cube_texture, vec3(texture_coordinates));
}
