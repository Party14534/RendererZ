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

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 FragPos,
        vec3 Albedo, float Specular, float sao) {
    vec3 lightDir = normalize(-light.direction);

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.);

    // Specular
    vec3 halfwayDir = normalize(lightDir + viewDir); // Blinn-Phong
    //vec3 reflectDir = reflect(-lightDir, normal); // Phong
    float spec = pow(max(dot(normal, halfwayDir), 0.), 32.);

    vec3 ambient = light.ambient * Albedo * sao;
    vec3 diffuse = light.diffuse * (diff * Albedo);
    vec3 specular = light.specular * (spec * Specular * Albedo);

    vec4 fragPosLightSpace = lightSpaceMatrix_z * vec4(FragPos + normal * .1, 1.);
    vec3 projCoords = fragPosLightSpace.xyz * .5 + .5;

    float lit = texture(gDirShadowMap, projCoords); 

    return (ambient + lit * (diffuse + specular));
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos,
        vec3 viewDir, vec3 Albedo, float Specular, float sao) {
    vec3 lightDir = normalize(light.pos - fragPos);

    float diff = max(dot(normal, lightDir), 0.);

    // Specular
    vec3 halfwayDir = normalize(lightDir + viewDir); // Blinn-Phong
    //vec3 reflectDir = reflect(-lightDir, normal); // Phong
    float spec = pow(max(dot(normal, halfwayDir), 0.), 32.);

    float distance = length(light.pos - fragPos);
    float attenuation = 1.0 / (light.attenuation.x
            + light.attenuation.y * distance
            + light.attenuation.z * (distance * distance));

    vec3 ambient = light.ambient * Albedo * sao;

    vec3 diffuse = light.diffuse * (diff * Albedo);
    vec3 specular = light.specular * (spec * Specular * Albedo);

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 calcSpotLight() {
 return vec3(0.f);
}

void main()
{
    vec3 FragPos = texture(gPosition, TexCoord).rgb;
    vec3 Normal =  texture(gNormal, TexCoord).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoord).rgb;
    float Specular = texture(gAlbedoSpec, TexCoord).a;

    vec3 viewDir = normalize(view_pos_z - FragPos);

    // LIGHT CODE
    float sao = texture(gSAOBlur, TexCoord).r;
    vec3 result = calcDirLight(dirLight_z, Normal, viewDir, FragPos, Albedo, Specular, sao);

    for(int i = 0; i < pointLightCnt_z; i++) {
        result += calcPointLight(pointLights_z[i], Normal, FragPos, viewDir, Albedo, Specular, sao);
    }

    FragColor = showSao_z ? vec4(vec3(sao), 1.) : vec4(result, 1.);
}
