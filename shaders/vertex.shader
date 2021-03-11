#version 450 core
// Taking in position
layout (location = 0) in vec4 position;
layout (location = 1) in vec4 inColor;

uniform mat4 transform;
uniform bool cancel;
out vec4 fragColor;

void main() {
   // Uses position input to set the position simply
   // Normalizing the w demension as well
   gl_Position = position * transform / position.w;
   fragColor = inColor;
}
