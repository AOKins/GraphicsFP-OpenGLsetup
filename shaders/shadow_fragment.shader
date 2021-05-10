#version 430 core

layout(location = 0) out vec3 fragColor;

void main() {
    fragColor = vec3(gl_FragCoord.z);
    gl_FragDepth = gl_FragCoord.z;
}