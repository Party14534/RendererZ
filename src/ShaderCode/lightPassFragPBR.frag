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
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight_z;

in vec2 TexCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gMetallic;
uniform sampler2D gRoughness;
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
    float denom = (normalHalfwayDot * normalHalfwayDot) * (nom - 1.) + 1.;
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
vec3 FresnelSchlick(float cosTheta/*, vec3 FZero*/) {
    vec3 FZero = vec3(.04);
    FZero = mix(FZero, Albedo.rgb, Metallic);
    
    return FZero + (1. - FZero) * pow(1. - cosTheta, 5.0);
}

vec3 CTBRDF(vec3 FragPos, vec3 Wi, vec3 viewDir, float roughness) {
    vec3 halfway = normalize(Wi + viewDir);

    float kS = calcSpecular();
    float kD = 1. - kS;

    // Calculate lambert
    vec3 lambert = Albedo * PI_RECIP;

    // Calculate Cook-Torrance
    float normalDistribution = GGXTR(Normal, halfway,
            roughness); // Trowbridge-Reitz GGX

    vec3 fresnelEq = FresnelSchlick(dot(Normal, halfway)); // Schlick-Fresnel
    
    float geometryFunc = GeomSmith(Normal, viewDir, 
            Wi, calcDirectRoughnessK(roughness)); // Smith's Schlick-GGX

    vec3 nom = normalDistribution * fresnelEq * geometryFunc;
    float denom = 4 * dot(viewDir, Normal) * dot(Wi, Normal);

    vec3 cookTorrance = nom / denom;
    
    return kD*lambert + cookTorrance;
}

float calcAttenuation(vec3 attenuation, float distance) {
    return 1.0 / (attenuation.x
            + attenuation.y * distance
            + attenuation.z * (distance * distance));
}

vec3 Radiance(vec3 fragPos, vec3 Wi, PointLight light) {
    vec3 lightCol = light.color;
    float cosTheta = max(dot(Normal, Wi), 0.);
    float attenuation = calcAttenuation(light.attenuation, length(light.pos - fragPos));
    
    return lightCol * attenuation * cosTheta;
}

void main()
{
    vec3 viewDir = normalize(view_pos_z - FragPos);

    // LIGHT CODE
    float sao = texture(gSAOBlur, TexCoord).r;

    vec3 sum = vec3(0.);
    float dW = 1. / pointLightCnt_z;
    for(int i = 0; i < pointLightCnt_z; i++) {
        // Calculate incoming light direction
        vec3 Wi = normalize(pointLights_z[i].pos - FragPos);

        // Riemann sum for the rendering equation
        sum += CTBRDF(FragPos, Wi, viewDir, Roughness) * // Bi-directional Reflective Distribution Function
            Radiance(FragPos, Wi, pointLights_z[i]) * // Incoming light
            dot(Normal, Wi) *
            dW;
    }
    sum = sum / pointLightCnt_z;

    FragColor = showSao_z ? vec4(vec3(sao), 1.) : vec4(sum, 1.);
}
