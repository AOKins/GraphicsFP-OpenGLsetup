#version 450 core

out vec4 color;
in vec3 myPosition;

void main(void) {
   color = vec4(0.5f, 0.5f, 0.5f, 1.0f);
   color.x += myPosition.x / 2.0f;
   color.y += myPosition.y / 2.0f;
   color.z += myPosition.z / 2.0f;
}