#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out float gMetallic;
layout (location = 4) out float gRoughness;

struct Material {
    sampler2D albedo;
    sampler2D normal;
    sampler2D metallic;
    sampler2D roughness;
};

uniform Material material_z;

in mat3 TBN;
in vec3 FragPos;
in vec2 TexCoord;

uniform bool usingTex_z;
uniform vec3 color_z;

uniform bool usingNormalMap_z;

uniform samplerCube skyBox_z;
uniform bool usingSkyBox_z;

void main()
{
    gPosition = FragPos;

    gNormal = usingNormalMap_z
        ? normalize(TBN * (texture(material_z.normal, TexCoord).rgb * 2.0 - 1.0))
        : normalize(TBN[2]);

    vec4 col = usingTex_z ? texture(material_z.albedo, TexCoord) : vec4(1.);
    gAlbedoSpec.rgb = col.rgb;
    gAlbedoSpec.rgb *= color_z;

    gMetallic.r = texture(material_z.metallic, TexCoord);
    gRoughness.r = texture(material_z.roughness, TexCoord);
}
