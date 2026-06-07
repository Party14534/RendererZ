#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform vec4 color_z;
uniform vec4 light_color_z;

uniform vec3 light_pos_z;
uniform vec3 view_pos_z;

uniform bool usingTex_z;

uniform sampler2D tex;

void main()
{
    vec4 objCol = usingTex_z ? texture(tex, TexCoord) * color_z : color_z;

    float ambientStrength = 0.1;
    vec3 ambient = light_color_z.xyz * ambientStrength;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light_pos_z - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light_color_z.xyz * diff;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(view_pos_z - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * light_color_z.xyz;

    vec3 result = vec3(objCol.xyz) * (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}
