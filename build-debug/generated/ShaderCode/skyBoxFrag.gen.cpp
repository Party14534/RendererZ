// Generated from /Users/zachariahdellimore/Code/RendererZ/src/Shaders/skyBoxFrag.frag by EmbedShader.cmake - do not edit by hand.
#include <string>

std::string skyBoxFrag = R"GLSL(#version 330 core

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skyBox_z;

void main() {
    FragColor = texture(skyBox_z, TexCoords);
}
)GLSL";
