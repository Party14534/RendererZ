#version 330 core
out vec4 FragColor;

in vec3 vertColor;
in vec2 TexCoord;

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform float dt;
uniform vec4 color;

void main()
{
    FragColor = mix(texture(tex1, TexCoord) * color,
            texture(tex2, TexCoord) * color, sin(dt));
}
