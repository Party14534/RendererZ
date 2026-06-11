#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 pos;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform vec3 view_pos_z;

uniform bool usingTex_z;

uniform sampler2D tex;

uniform Material material_z;
uniform Light light_z;

void main()
{
    vec4 objCol = usingTex_z ? texture(tex, TexCoord) : vec4(1.);

    // LIGHT CODE
    vec3 ambient = light_z.ambient * (material_z.ambient * objCol.xyz);

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light_z.pos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light_z.diffuse * (diff * material_z.diffuse * objCol.xyz);

    vec3 viewDir = normalize(view_pos_z - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_z.shininess);
    vec3 specular = light_z.specular * (spec * material_z.specular * objCol.xyz);

    // Create smaller light coming from camera
    //float vDiff = max(dot(norm, viewDir), 0.0) * .2;
    //vec3 vDiffuse = light_z.diffuse * (vDiff * material_z.diffuse * objCol.xyz);

    vec3 result = ambient + diffuse + specular;
    //result += vDiffuse;
    FragColor = vec4(result, 1.0);
}
