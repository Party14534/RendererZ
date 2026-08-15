// Generated from /Users/zachariahdellimore/Code/RendererZ/src/Shaders/unused/pointFrag.frag by EmbedShader.cmake - do not edit by hand.
#include <string>

std::string pointFrag = R"GLSL(#version 330 core

out vec4 FragColor;

in vec3 Color;
in vec3 FragPos;
in vec2 TexCoord;

uniform sampler2D tex;
uniform bool usingTex_z;

void main()
{
    vec4 col = usingTex_z ? texture(tex, TexCoord) : vec4(Color, 1.0);

    FragColor = col;
}
)GLSL";
