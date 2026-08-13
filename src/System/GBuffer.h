#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Math/math.h"
#include "Objects/Texture.h"

struct GBuffer {
    u32 ID;
    u32 depthRBO;
    Texture pos, norm, color;

    GBuffer();
    void init(u32 width, u32 height);
    void bind();
    void bindTextures();
    void unbind();
};

#endif
