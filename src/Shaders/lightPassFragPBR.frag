#version 330 core

out vec4 FragColor;

struct PointLight {
    vec3 pos;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 attenuation;
};
#define MAX_POINT_LIGHTS 800
uniform int pointLightCnt_z;

// UBO instead of a plain uniform array: much higher size limit.
layout(std140) uniform PointLightBlock {
    PointLight pointLights_z[MAX_POINT_LIGHTS];
};

struct DirLight {
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight_z;

in vec2 TexCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gSAOBlur;

uniform sampler2DShadow gDirShadowMap;

uniform bool showSao_z;
uniform vec2 resolution_z;
uniform vec3 view_pos_z;
uniform mat4 projection_z;
uniform mat4 lightSpaceMatrix_z;

#define PI 3.141592653589

vec3 FragPos = texture(gPosition, TexCoord).rgb;
vec3 Normal =  texture(gNormal, TexCoord).rgb;
vec3 Albedo = texture(gAlbedoSpec, TexCoord).rgb;
float Specular = texture(gAlbedoSpec, TexCoord).a;

float CTBRDF(vec3 FragPos, vec3 Wi, vec3 viewDir, float roughness) {
    float kS = calcSpecular();
    float kD = 1. - kS;

    // Calculate lambert
    float lambert = Albedo / PI;

    // Calculate Cook-Torrance
    float normalDistribution;
    float geometryFunc;
    float fresnelEq;
    float nom = normalDistribution * geometyFunc * fresnelEq
    float denom = 4 * dot(viewDir, Normal) * dot(Wi, Normal);

    float cookTorrance = nom / denom;
    
    return kD*lambert + kS*cookTorrance;
}

void main()
{
    FragPos = texture(gPosition, TexCoord).rgb;
    Normal =  texture(gNormal, TexCoord).rgb;
    Albedo = texture(gAlbedoSpec, TexCoord).rgb;
    Specular = texture(gAlbedoSpec, TexCoord).a;

    vec3 viewDir = normalize(view_pos_z - FragPos);

    // LIGHT CODE
    float sao = texture(gSAOBlur, TexCoord).r;
    //vec3 result = calcDirLight(dirLight_z, Normal, viewDir, FragPos, Albedo, Specular, sao);

    float sum = 0.;
    float dW = 1. / pointLightCnt_z;
    for(int i = 0; i < pointLightCnt_z; i++) {
        // Calculate incoming light direction
        vec3 Wi = normalize(pointsLights_z[i].pos - FragPos);

        // Riemann sum for the rendering equation
        sum += CTBRDF(FragPos, Wi, viewDir, roughness) * // Bi-directional Reflective Distribution Function
            Radiance(FragPos, Wi) * // Incoming light
            dot(Normal, Wi) *
            dW;
        result += calcPointLight(pointLights_z[i], Normal, FragPos, viewDir, Albedo, Specular, sao);
    }

    FragColor = showSao_z ? vec4(vec3(sao), 1.) : vec4(result, 1.);
}
