#version 450 core

out vec4 color;

uniform sampler2D twoDTex; 

in vec2 vs_uv;

void main(void) {
   color = texture(twoDTex, vs_uv);
}
