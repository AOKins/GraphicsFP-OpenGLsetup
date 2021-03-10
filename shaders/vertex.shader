#version 450 core
// Taking in position
layout (location = 0) in vec3 position;
out vec3 fragColor;
// Uses position input to set the position simply, and color output is hardcoded white solid
void main() {
   gl_Position = vec4(position.x, position.y, position.z, 1.0f);

   fragColor = vec3(position.x, position.y, position.z);
}
