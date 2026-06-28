#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct PointLight {
    vec3 pos;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 attenuation;
};
#define MAX_POINT_LIGHTS 4
uniform int pointLightCnt_z;
uniform PointLight pointLights_z[MAX_POINT_LIGHTS];

struct DirLight {
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight_z;

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform vec3 view_pos_z;

uniform bool usingTex_z;

uniform sampler2D tex;

uniform Material material_z;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.);

    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.), material_z.shininess);

    vec4 objCol = usingTex_z ? texture(tex, TexCoord) : vec4(1.);
    vec3 ambient = light.ambient * (material_z.ambient * objCol.xyz);
    vec3 diffuse = light.diffuse * (diff * material_z.diffuse * objCol.xyz);
    vec3 specular = light.specular * (spec * material_z.specular * objCol.xyz);

    return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.pos - fragPos);

    float diff = max(dot(normal, lightDir), 0.);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.), material_z.shininess);

    float distance = length(light.pos - fragPos);
    float attenuation = 1.0 / (light.attenuation.x
            + light.attenuation.y * distance
            + light.attenuation.z * (distance * distance));

    vec4 objCol = usingTex_z ? texture(tex, TexCoord) : vec4(1.);
    vec3 ambient = light.ambient * (material_z.ambient * objCol.xyz);
    vec3 diffuse = light.diffuse * (diff * material_z.diffuse * objCol.xyz);
    vec3 specular = light.specular * (spec * material_z.specular * objCol.xyz);

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
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(view_pos_z - FragPos);
    // LIGHT CODE

    vec3 result = calcDirLight(dirLight_z, norm, viewDir);

    for(int i = 0; i < pointLightCnt_z; i++) {
        result += calcPointLight(pointLights_z[i], norm, FragPos, viewDir);
    }

    // Create smaller light coming from camera
    //float vDiff = max(dot(norm, viewDir), 0.0) * .2;
    //vec3 vDiffuse = light.diffuse * (vDiff * material_z.diffuse * objCol.xyz);

    FragColor = vec4(result, 1.0);
}
