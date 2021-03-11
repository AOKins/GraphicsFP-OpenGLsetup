#version 450 core

out vec4 color;

in vec4 fragColor;

void main(void) {
   color = fragColor;
}
