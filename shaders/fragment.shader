// Fragment shader with attempt to simulate light sources
// Methodologies based on Scott Griffith's example
#version 450 core

out vec4 color;

uniform sampler2D twoDTex; 
uniform sampler2D shadowMap;

in vec2 vs_uv;     // UV coordinate
in vec4 vs_vertex; // Where the vertex is in world space
in vec3 vs_normal; // The normal for the vertex (in world orientation)
in vec4 L_vertex;

uniform int LightCount;
uniform vec4 Lpos[32];
uniform vec3 Lcolor[32];
uniform float Linten[32];
uniform float Kambient[32];
uniform float Kdiffuse[32];
uniform float Kspecular[32];
uniform float U_alpha[32];

// Where the camera is (used in light model)
uniform vec3 cameraPos;

// Method for determining the ambient light
vec3 calcAmbient(vec3 lightColor, float coefficient) {
    vec3 result = lightColor * coefficient;
    return result;
}

vec3 calcSpecular(vec3 lightColor, float coeff, float alpha, vec3 L, float cosTheta) {
    vec3 v = normalize(cameraPos-vs_vertex.xyz);
    vec3 r = 2 * cosTheta * normalize(vs_normal) - L;
    float cosPhi = max(dot(v,r),0.0);

    vec3 result = lightColor * coeff * pow(cosPhi, alpha);
    return result;
}

vec3 calcDiffuse(vec3 lightColor, float coeff, vec3 L, float cosTheta) {
    vec3 result = lightColor * coeff * cosTheta;
    return result;
}

float shadowValue(vec4 position) {
    vec3 projPos = (position.xyz/position.w)*0.5 + 0.5;
    float closestDepth = texture(shadowMap, projPos.xy).r;
    float thisDepth = position.z;
    if (thisDepth <= closestDepth) {
        return 1.0f;
    }
    else {
        return 0.0f;
    }
}

void main(void) {
    // Final I value from all light sources
    vec3 I_result;
    vec3 I; // Resulting illumination value from a light
    // Components of light to be derived then added into resulting value
    vec3 I_ambient, I_diffuse, I_specular;

    //Material properties
    float K_ambient;  // Ambient reflection coeff.
    float K_diffuse;  // Diffuse reflection coeff.
    float K_specular; // Specular reflection coeff.
    float alpha;      // Specular exponent (m_gls)
    //   These could be pulled in via attributes, but for now, we will define them here
    vec3 L_ambient = vec3(1.0, 1.0, 1.0); // around the scene light color
    vec3 L_diffuse = vec3(1.0, 1.0, 1.0);  // Scattered light color
    vec3 L_specular = vec3(1.0, 1.0, 1.0); // Color of shininess of object

    vec4 lightPos;
    vec3 lightColor;
    float lightIntensity;

    // Get number of lights, capped at 32
    int maxLights = min(LightCount,32);

    for(int i = 0; i < maxLights; i++) {
        // Get the values for this light
        lightPos = Lpos[i];
        lightColor = Lcolor[i];
        lightIntensity = Linten[i];
        K_ambient = Kambient[i];
        K_diffuse = Kdiffuse[i];
        K_specular = Kspecular[i];
        alpha = U_alpha[i];

        // Deriving a normalized vector from the vertex point to the light source
        vec3 L = normalize(vec3(lightPos.xyz) - vec3(vs_vertex.xyz));
        float cosTheta = max(dot(L, normalize(vs_normal) ),0);

        // Found that currently, not restricting diffuse by cosTheta>0 condition makes multiple lights overlap better
        I_diffuse = calcDiffuse(L_diffuse, K_diffuse, L, cosTheta);

        if (cosTheta > 0) { 
            I_specular = calcSpecular(L_specular, K_specular, alpha, L, cosTheta);
        }

        I_ambient = calcAmbient(L_ambient, K_ambient);
        I = (lightIntensity * (I_diffuse + I_specular + I_ambient) / length(lightPos - vs_vertex));
        I = I * lightColor * shadowValue(L_vertex);
        I_result = (I_result + I);
    }
    
    // Now apply the resulting texture and light values to the output color    
    vec4 textColor = texture(twoDTex, vs_uv);
    color = textColor * vec4(min(I_result+I_ambient,vec3(1,1,1)), 1.0);
}
