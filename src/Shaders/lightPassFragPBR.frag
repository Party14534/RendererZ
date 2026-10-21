#version 330 core

out vec4 FragColor;

/*
 * LIGHTING
 */

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
uniform smapler2D gMetallic;
uniform smapler2D gRoughness;
uniform sampler2D gSAOBlur;

uniform sampler2DShadow gDirShadowMap;

uniform bool showSao_z;
uniform vec2 resolution_z;
uniform vec3 view_pos_z;
uniform mat4 projection_z;
uniform mat4 lightSpaceMatrix_z;

#define PI 3.141592653589
#define PI_RECIP 0.318309886

vec3 FragPos = texture(gPosition, TexCoord).rgb;
vec3 Normal =  texture(gNormal, TexCoord).rgb;
vec3 Albedo = texture(gAlbedoSpec, TexCoord).rgb;
float Specular = texture(gAlbedoSpec, TexCoord).a;
float Metallic = texture(gMetallic, TexCoord).r;
float Roughness = texture(gRoughness, TexCoord).r;

float calcIBLRoughnessK(float roughness) {
    return roughness * roughness * .5;
}

float calcDirectRoughnessK(float roughness) {
    return ((roughness - 1.) * (roughness - 1.)) * .125;
}

// Trowbridge-Reitz GGX
// Approximates the relative area of microfacets exactly aligned
// to the halfway vector
float GGXTR(vec3 normal, vec3 halfway, float roughness) {
    float normalHalfwayDot = max(dot(normal, halfway), 0.);

    float nom = roughness * roughness;
    float denom = (normalHalfway * normalHalfway) * (nom - 1.) + 1.;
    denom = PI * denom * denom;

    return nom / denom;
}

// Schlick GGX Geometry function
// Approximates the relative surface area where micro surface details 
// overshadow each other
// The roughness changes whether this i0.318309886s from IBL to direct
float GeomSchlickGGX(float NdotV, float roughnessK) {
    float nom = NdotV;
    float denom = NdotV * (1. - roughnessK) + roughnessK;

    return nom / denom;
}

float GeomSmith(vec3 normal, vec3 view, vec3 Wi, float roughnessK) {
    float NdotV = max(dot(normal, view), 0.); // Try not maxing here because should alr be occluded if it was
    float NdotL = max(dot(normal, Wi), 0.);
    float ggx1 = GeomSchlickGGX(NdotV, roughnessK);
    float ggx2 = GeomSchlickGGX(NdotL, roughnessK);

    return ggx1 * ggx2;
}

// Fresnel-Schlick
// Ratio of light that gets reflected over the light that gets refracted.
// Varies over the angle we're looking at the surface
float FresnelSchlick(float cosTheta, vec3 FZero) {
    //vec3 F0 = vec3(.04);
    //F0 = mix(F0, Albedo.rgb, metalness);
    
    return FZero + (1. - FZero) * pow(1. - cosTheta, 5.0);
}

float CTBRDF(vec3 FragPos, vec3 Wi, vec3 viewDir, float roughness) {
    vec3 halfway = normalize(Wi, viewDir);

    float kS = calcSpecular();
    float kD = 1. - kS;

    // Calculate lambert
    float lambert = Albedo * PI_RECIP;

    // Calculate Cook-Torrance
    float normalDistribution = GGXTR(Normal, halfway,
            roughness); // Trowbridge-Reitz GGX

    float fresnelEq = FresnelSchlick(dot(Normal, halfway),
            material_z.FZero); // Schlick-Fresnel
    
    float geometryFunc = GeomSmith(Normal, viewDir, 
            Wi, calcDirectRoughnessK(roughness)); // Smith's Schlick-GGX

    float nom = normalDistribution * fresnelEq * geometyFunc;
    float denom = 4 * dot(viewDir, Normal) * dot(Wi, Normal);

    float cookTorrance = nom / denom;
    
    return kD*lambert + cookTorrance;
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
