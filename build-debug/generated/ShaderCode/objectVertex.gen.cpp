// Generated from /Users/zachariahdellimore/Code/RendererZ/src/Shaders/objectVertex.vert by EmbedShader.cmake - do not edit by hand.
#include <string>

std::string objectVertex = R"GLSL(#version 330

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out vec2 TexCoord;
out vec3 FragPos;
out mat3 TBN;

uniform mat4 model_z;
uniform mat4 view_z;
uniform mat4 projection_z;
uniform vec2 uvScale_z;

void main()
{
    gl_Position = projection_z * view_z * model_z * vec4(aPos, 1.0);
    FragPos = vec3(model_z * vec4(aPos, 1.0));
    TexCoord = aTexCoord * uvScale_z;

    vec3 T = normalize(vec3(model_z * vec4(aTangent, 0.)));
    vec3 B = normalize(vec3(model_z * vec4(aBitangent, 0.)));
    vec3 N = normalize(vec3(model_z * vec4(aNormal, 0.)));
    TBN = mat3(T, B, N);
}
)GLSL";
