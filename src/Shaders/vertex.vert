#version 330

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aCol;
layout(location = 2) in vec2 aTexCoord;

out vec3 vertColor;
out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vertColor = aCol;
    TexCoord = aTexCoord;
}
