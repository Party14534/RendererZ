#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec2 gMetallicRoughness;

struct Material {
    sampler2D albedo;
    sampler2D normal;
    sampler2D metallicRoughness;

    vec4 color;
    float metallicFactor;
    float roughnessFactor;

    bool usingAlbedo;
    bool usingNormal;
    bool usingMetallicRoughness;
};
uniform Material material_z;

in mat3 TBN;
in vec3 FragPos;
in vec2 TexCoord;

/*uniform samplerCube skyBox_z;
uniform bool usingSkyBox_z;*/

void main()
{
    gPosition = FragPos;

    gNormal = material_z.usingNormal
        ? normalize(TBN * (texture(material_z.normal, TexCoord).rgb * 2.0 - 1.0))
        : normalize(TBN[2]);

    vec4 col = material_z.usingAlbedo 
        ? texture(material_z.albedo, TexCoord) 
        : vec4(1.);

    gAlbedoSpec = col * material_z.color;

    vec2 metallicRoughness = material_z.usingMetallicRoughness 
        ? texture(material_z.metallicRoughness, TexCoord).bg
        : vec2(1.);

    gMetallicRoughness = metallicRoughness * vec2(material_z.metallicFactor,
            material_z.roughnessFactor);
}
