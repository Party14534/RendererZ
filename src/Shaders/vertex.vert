#version 330

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aCol;
layout(location = 2) in vec2 aTexCoord;

out vec3 vertColor;
out vec2 TexCoord;

uniform mat4 transform_z;

void main()
{
    gl_Position = transform_z * vec4(aPos, 1.0);
    vertColor = aCol;
    TexCoord = aTexCoord;
}
