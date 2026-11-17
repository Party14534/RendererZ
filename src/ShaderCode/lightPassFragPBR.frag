#version 330 core

out vec4 FragColor;

/*
 * LIGHTING
 */

struct PointLight {
    vec3 pos;
    vec3 attenuation;
    vec3 color;
};
#define MAX_POINT_LIGHTS 800
uniform int pointLightCnt_z;

// UBO instead of a plain uniform array: much higher size limit.
layout(std140) uniform PointLightBlock {
    PointLight pointLights_z[MAX_POINT_LIGHTS];
};

struct DirLight {
    vec3 direction;
    vec3 color;
};
uniform DirLight dirLight_z;

in vec2 TexCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gMetallicRoughness;

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
//float Specular = texture(gAlbedoSpec, TexCoord).a;
float Metallic = texture(gMetallicRoughness, TexCoord).r;
float Roughness = texture(gMetallicRoughness, TexCoord).g;
float epsilon = 0.0001;

float calcIBLRoughnessK(float roughness) {
    return roughness * roughness * .5;
}

float calcDirectRoughnessK(float roughness) {
    return ((roughness - 1.) * (roughness - 1.)) * .125;
}

float calcRoughnessK(float roughness) {
    float r = roughness + 1.;
    return (r * r) * .125;
}

// Trowbridge-Reitz GGX
// Approximates the relative area of microfacets exactly aligned
// to the halfway vector
float GGXTR(vec3 normal, vec3 halfway, float roughness) {
    float normalHalfwayDot = max(dot(normal, halfway), 0.);

    float nom = roughness * roughness;
    float denom = (normalHalfwayDot * normalHalfwayDot) * (nom - 1.) + 1.;
    denom = (PI * denom * denom) + epsilon;

    return nom / denom;
}

// Schlick GGX Geometry function
// Approximates the relative surface area where micro surface details 
// overshadow each other
// The roughness changes whether this i0.318309886s from IBL to direct
float GeomSchlickGGX(float NdotV, float roughness) {
    float k = calcRoughnessK(roughness);

    float nom = NdotV;
    float denom = NdotV * (1. - k) + k;

    return nom / denom;
}

float GeomSmith(float NdotV, float NdotL, float roughness) {
    float ggx1 = GeomSchlickGGX(NdotV, roughness);
    float ggx2 = GeomSchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

// Fresnel-Schlick
// Ratio of light that gets reflected over the light that gets refracted.
// Varies over the angle we're looking at the surface
vec3 FresnelSchlick(float cosTheta/*, vec3 FZero*/) {
    vec3 FZero = vec3(.04);
    FZero = mix(FZero, Albedo.rgb, Metallic);
    
    return FZero + (1. - FZero) * pow(clamp(1. - cosTheta, 0., 1.), 5.0);
}

float calcSpecular() {
    return 0.3;
}

vec3 CTBRDF(vec3 FragPos, vec3 Wi, vec3 viewDir, float roughness) {
    vec3 halfway = normalize(Wi + viewDir);

    // Calculate lambert
    vec3 lambert = Albedo * PI_RECIP;

    // Calculate Cook-Torrance
    float normalDistribution = GGXTR(Normal, halfway,
            roughness); // Trowbridge-Reitz GGX

    vec3 fresnelEq = FresnelSchlick(max(dot(halfway, viewDir), 0.)); // Schlick-Fresnel

    vec3 kS = fresnelEq;
    vec3 kD = 1. - kS;
    kD *= 1. - Metallic;
    
    float NdotV = max(dot(Normal, viewDir), 0.); // Try not maxing here because should alr be occluded if it was
    float NdotL = max(dot(Normal, Wi), 0.);
    float geometryFunc = GeomSmith(NdotV, NdotL, roughness); // Smith's Schlick-GGX

    vec3 nom = normalDistribution * fresnelEq * geometryFunc;
    float denom = 4. * NdotV * NdotL + epsilon;

    vec3 cookTorrance = nom / denom;
    
    return (kD*lambert + cookTorrance) * NdotL;
}

float calcAttenuation(float distance) {
    return 1.0 / (distance * distance);
}

vec3 Radiance(vec3 fragPos, vec3 Wi, PointLight light) {
    vec3 lightCol = light.color;
    float cosTheta = max(dot(Normal, Wi), 0.);
    float attenuation = calcAttenuation(length(light.pos - fragPos));
    
    return lightCol * attenuation * cosTheta;
}

vec3 Radiance(vec3 fragPos, vec3 Wi, DirLight light) {
    vec3 lightCol = light.color;
    float cosTheta = max(dot(Normal, Wi), 0.);
    
    return lightCol * cosTheta;
}

void main()
{
    vec3 viewDir = normalize(view_pos_z - FragPos);

    // LIGHT CODE
    float sao = texture(gSAOBlur, TexCoord).r;

    vec3 sum = vec3(0.);

    // Directional light
    vec3 Wi = normalize(-dirLight_z.direction);

    sum += CTBRDF(FragPos, Wi, viewDir, Roughness) * // Bi-directional Reflective Distribution Function
        Radiance(FragPos, Wi, dirLight_z); // Incoming light

    // PointLight
    for(int i = 0; i < pointLightCnt_z; i++) {
        // Calculate incoming light direction
        vec3 Wi = normalize(pointLights_z[i].pos - FragPos);

        // Riemann sum for the rendering equation
        sum += CTBRDF(FragPos, Wi, viewDir, Roughness) * // Bi-directional Reflective Distribution Function
            Radiance(FragPos, Wi, pointLights_z[i]); // Incoming light
    }
    sum += (0.03 * Albedo) * sao;

    FragColor = showSao_z ? vec4(vec3(sao), 1.) : vec4(sum, 1.);
}
