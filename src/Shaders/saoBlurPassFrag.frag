#version 330 core

layout (location = 0) out vec2 gSAOBlur;

in vec2 TexCoord;

uniform sampler2D gSAO;
uniform vec2 saoBlurDir_z;
uniform vec2 resolution_z;

const int RADIUS = 4;
const float DEPTH_SIGMA = 1.;

void main()
{
    vec2 center = texture(gSAO, TexCoord).rg;
    float centerDepth = center.g;

    float result = center.r;
    float totalWeight = 1.;

    for (int i = 1; i <= RADIUS; i++) {
        vec2 offset = saoBlurDir_z * float(i) / resolution_z;

        vec2 tapPos = texture(gSAO, TexCoord + offset).rg;
        float depthDiffPos = tapPos.g - centerDepth;
        float weightPos = exp(-(depthDiffPos * depthDiffPos) / (2. * DEPTH_SIGMA * DEPTH_SIGMA));
        result += tapPos.r * weightPos;
        totalWeight += weightPos;

        vec2 tapNeg = texture(gSAO, TexCoord - offset).rg;
        float depthDiffNeg = tapNeg.g - centerDepth;
        float weightNeg = exp(-(depthDiffNeg * depthDiffNeg) / (2. * DEPTH_SIGMA * DEPTH_SIGMA));
        result += tapNeg.r * weightNeg;
        totalWeight += weightNeg;
    }

    gSAOBlur = vec2(result / totalWeight, centerDepth);
}
