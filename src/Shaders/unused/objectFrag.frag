#version 330 core

out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    float reflectivity;
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

in mat3 TBN;
in vec3 FragPos;
in vec2 TexCoord;

uniform vec3 view_pos_z;

uniform bool usingTex_z;

uniform sampler2D tex;

uniform Material material_z;

uniform bool usingNormalMap_z;
uniform sampler2D normalMap;

uniform samplerCube skyBox_z;
uniform bool usingSkyBox_z;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.);

    // Specular
    vec3 halfwayDir = normalize(lightDir + viewDir); // Blinn-Phong
    //vec3 reflectDir = reflect(-lightDir, normal); // Phong
    float spec = pow(max(dot(normal, halfwayDir), 0.), material_z.shininess);

    vec4 objCol = usingTex_z ? texture(tex, TexCoord) : vec4(1.);
    vec3 ambient = light.ambient * (material_z.ambient * objCol.xyz);
    vec3 diffuse = light.diffuse * (diff * material_z.diffuse * objCol.xyz);
    vec3 specular = light.specular * (spec * material_z.specular * objCol.xyz);

    return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.pos - fragPos);

    float diff = max(dot(normal, lightDir), 0.);

    // Specular
    vec3 halfwayDir = normalize(lightDir + viewDir); // Blinn-Phong
    //vec3 reflectDir = reflect(-lightDir, normal); // Phong
    float spec = pow(max(dot(normal, halfwayDir), 0.), material_z.shininess);

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
    vec3 norm = usingNormalMap_z
        ? normalize(TBN * (texture(normalMap, TexCoord).rgb * 2.0 - 1.0))
        : normalize(TBN[2]);
    vec3 viewDir = normalize(view_pos_z - FragPos);
    // LIGHT CODE

    vec3 result = calcDirLight(dirLight_z, norm, viewDir);

    for(int i = 0; i < pointLightCnt_z; i++) {
        result += calcPointLight(pointLights_z[i], norm, FragPos, viewDir);
    }

    // Create smaller light coming from camera
    //float vDiff = max(dot(norm, viewDir), 0.0) * .2;
    //vec3 vDiffuse = light.diffuse * (vDiff * material_z.diffuse * objCol.xyz);

    // Add in skybox
    vec3 I = normalize(FragPos - view_pos_z);
    vec3 R = refract(I, norm, 1.00 / 1.52);
    result += (usingSkyBox_z) ? texture(skyBox_z, R).rgb * material_z.reflectivity : vec3(0.f);

    FragColor = vec4(result, 1.0);
}
