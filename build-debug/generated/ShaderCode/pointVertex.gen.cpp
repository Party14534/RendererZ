// Generated from /Users/zachariahdellimore/Code/RendererZ/src/Shaders/unused/pointVertex.vert by EmbedShader.cmake - do not edit by hand.
#include <string>

std::string pointVertex = R"GLSL(#version 330

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 Color;
out vec3 FragPos;

uniform mat4 view_z;
uniform mat4 projection_z;
uniform float pointSize_z;

void main()
{
    gl_Position = projection_z * view_z * vec4(aPos, 1.0);
    gl_PointSize = pointSize_z;
    TexCoord = aTexCoord;
    Color = aColor;
    FragPos = vec3(vec4(aPos, 1.0));
}
)GLSL";
