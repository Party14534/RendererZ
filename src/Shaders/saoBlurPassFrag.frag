#version 330 core

layout (location = 0) out float gSAOBlur;

in vec2 TexCoord;

uniform sampler2D gSAO;

void main()
{
    vec2 texelSize = 1.0 / vec2(textureSize(gSAO, 0));
    float result = 0.0;
    for (int x = -2; x < 2; ++x) 
    {
        for (int y = -2; y < 2; ++y) 
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(gSAO, TexCoord + offset).r;
        }
    }
    gSAOBlur = result / (4.0 * 4.0);
}
