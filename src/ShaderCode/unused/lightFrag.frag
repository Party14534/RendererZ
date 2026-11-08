#version 330 core
out vec4 FragColor;

uniform vec4 color_z;

void main()
{
    FragColor = color_z;
}
