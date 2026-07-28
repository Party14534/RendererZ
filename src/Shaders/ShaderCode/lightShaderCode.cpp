#include "../ShaderCode.h"

std::string lightFrag = R"(#version 330 core
out vec4 FragColor;

uniform vec4 color_z;

void main()
{
    FragColor = color_z;
}
)";

std::string lightVertex = R"(#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 model_z;
uniform mat4 view_z;
uniform mat4 projection_z;

void main() {
    gl_Position = projection_z * view_z * model_z * vec4(aPos, 1.0);
}
)";
