// Generated from /Users/zachariahdellimore/Code/RendererZ/src/Shaders/lightPassVertex.vert by EmbedShader.cmake - do not edit by hand.
#include <string>

std::string lightPassVertex = R"GLSL(#version 330

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out vec2 TexCoord;

uniform mat4 model_z;

void main()
{
    gl_Position = model_z * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
)GLSL";
