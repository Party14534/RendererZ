#version 330 core
out vec4 FragColor;

in vec3 vertColor;
in vec2 TexCoord;

uniform vec4 color_z;
uniform bool usingTex_z;
uniform sampler2D tex;

void main()
{
    FragColor = usingTex_z ? texture(tex, TexCoord) : color_z;
}
