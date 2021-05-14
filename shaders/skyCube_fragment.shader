#version 450 core                                                 

in vec4 tex_cords;
uniform samplerCube cube_texture;
uniform float sky_light_intensity;
uniform vec3 sky_light_color;
out vec4 color;

void main(void) {
    // Use cube_texture to interpolate the correct texture color
    color = sky_light_intensity * vec4(sky_light_color, 1.0) * texture(cube_texture, vec3(tex_cords));
}
