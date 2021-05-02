#version 450 core

out vec4 color;

uniform sampler2D twoDTex; 

in vec2 vs_uv;     // UV coordinate
in vec4 vs_vertex; // Where the vertex is in world space
in vec3 vs_normal; // The normal for the vertex (in world orientation)
in vec4 cameraS_vertex;

uniform vec4 Lpos;
uniform vec3 Lcolor;
uniform float Linten;
uniform int LightCount;


// Where the camera is (used in light model)
uniform vec3 cameraPos;


// Method for determining the ambient light
vec3 calcAmbient(vec3 lightColor, float coefficient) {
    vec3 result = lightColor * coefficient;
    return result;
}

vec3 calcSpecular(vec3 lightColor, float coeff, float alpha, vec3 L, float cosTheta) {
    vec3 result;
    
    vec3 v = normalize(cameraPos-vs_vertex.xyz);
    vec3 r = 2 * cosTheta * normalize(vs_normal) - L;

    float cosPhi = max(dot(v,r),0.0);

    result = lightColor * coeff * pow(cosPhi, alpha);

    return result;
}

vec3 calcDiffuse(vec3 lightColor, float coeff, vec3 L, float cosTheta) {
    vec3 result;
    result = lightColor * coeff * cosTheta;
    return result;
}

void main(void) {
    vec3 I; // Resulting illumination value

    // Components of light to be derived then added into resulting value
    vec3 I_ambient, I_diffuse, I_specular;

    //Material properties
    float K_ambient = 0.01; // Ambient reflection coeff.
    float K_diffuse = 0.55; // Diffuse reflection coeff.
    float K_specular = 0.95; // Specular reflection coeff.
    float alpha = 200.0;    // Specular exponent (m_gls)

    vec4  lightPos = Lpos;
    vec3  lightColor = Lcolor;
    float lightIntensity = Linten;

    //   These could be pulled in via attributes, but for now, we will define them here
    vec3 L_ambient = vec3(1.0, 1.0, 1.0); // around the scene light color
    vec3 L_diffuse = vec3(1.0, 1.0, 1.0);  // Scattered light color
    vec3 L_specular = vec3(1.0, 1.0, 1.0); // Color of shininess of object


    // Deriving a normalized vector from the vertex point to the light source
    vec3 L = normalize(vec3(lightPos.xyz) - vec3(vs_vertex.xyz));
    float cosTheta = max(dot(L, normalize(vs_normal) ),0);

    I_ambient = calcAmbient(L_ambient, K_ambient);
    
    if (cosTheta > 0) {
        I_specular = calcSpecular(L_specular, K_specular, alpha, L, cosTheta);
        I_diffuse = calcDiffuse(L_diffuse, K_diffuse, L, cosTheta);
    }
    
    I = (I_ambient + lightIntensity * (I_diffuse + I_specular) / length(lightPos - vs_vertex)) * lightColor;
    vec4 textColor = texture(twoDTex, vs_uv);
    color = textColor * vec4(I, 1.0);
}
