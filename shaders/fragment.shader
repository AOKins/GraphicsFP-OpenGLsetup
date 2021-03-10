#version 450 core

out vec4 color;
in vec3 fragColor;

void main(void) {
   color = vec4(fragColor.x, fragColor.y, fragColor.z, 1.0f);
}
