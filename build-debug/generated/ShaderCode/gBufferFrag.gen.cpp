// Generated from /Users/zachariahdellimore/Code/RendererZ/src/Shaders/gBufferFrag.frag by EmbedShader.cmake - do not edit by hand.
#include <string>

std::string gBufferFrag = R"GLSL(#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in mat3 TBN;
in vec3 FragPos;
in vec2 TexCoord;

uniform sampler2D tex;
uniform bool usingTex_z;

uniform float specular_z;

uniform bool usingNormalMap_z;
uniform sampler2D normalMap;

uniform samplerCube skyBox_z;
uniform bool usingSkyBox_z;

void main()
{
    gPosition = FragPos;

    gNormal = usingNormalMap_z
        ? normalize(TBN * (texture(normalMap, TexCoord).rgb * 2.0 - 1.0))
        : normalize(TBN[2]);

    gAlbedoSpec.rgb = usingTex_z ? texture(tex, TexCoord).rgb : vec3(1.);
    gAlbedoSpec.a = specular_z;
}
)GLSL";
