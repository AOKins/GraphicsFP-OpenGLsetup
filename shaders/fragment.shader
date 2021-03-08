#version 450 core

out vec4 color;
in vec3 myPosition;

void main(void) {
   color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
   color.x += myPosition.x;
   color.y += myPosition.y;
   color.z += myPosition.z;
}
