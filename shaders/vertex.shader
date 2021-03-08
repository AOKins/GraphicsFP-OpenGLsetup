#version 450 core
// Taking in position
layout (location = 0) in vec3 position;
out vec3 myPosition;
// Uses position input to set the position simply, and color output is hardcoded white solid
void main() {
   myPosition = position;
   if (gl_VertexID < 2) {
      gl_Position = vec4(position.x, -position.y, position.z, 1.0f);
   }
   else {
      gl_Position = vec4(position.x, -position.y, -position.z, 1.0f);
   }
}
