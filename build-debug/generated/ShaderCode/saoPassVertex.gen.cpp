// Generated from /Users/zachariahdellimore/Code/RendererZ/src/Shaders/saoPassVertex.vert by EmbedShader.cmake - do not edit by hand.
#include <string>

std::string saoPassVertex = R"GLSL(#version 330

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out vec2 TexCoord;

uniform mat4 mvp_z;

void main()
{
    //gl_Position = vec4(aPos, 1.0) * mvp_z;
    gl_Position = mvp_z * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
)GLSL";
