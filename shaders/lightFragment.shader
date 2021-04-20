#version 450 core

out vec4 color;

uniform sampler2D twoDTex; 

in vec2 vs_uv;     // UV coordinate
in vec4 vs_vertex; // Where the vertex is
in vec4 vs_normal; // The normal for the vertex (in world space)

void main(void) {
    vec3 I; // Resulting illumination value

    //Material properties
    float K_ambient = 0.65; // Ambient reflection coeff.
    float K_diffuse = 0.75; // Diffuse reflection coeff.
    float K_specular = 1.0; // Specular reflection coeff.
    float alpha = 200.0;    // Specular exponent (m_gls)







    color = texture(twoDTex, vs_uv);
}
