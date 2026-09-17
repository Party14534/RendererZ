#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Math/math.h"
#include "Objects/Texture.h"
#include "System/GraphicsApi/GraphicsApi.h"
#include <memory>

struct FrameBuffer {
    u32 ID, width, height, index;
    Texture tex;

    void init(TextureFormat internal, u32 width, u32 height,
            TextureFormat format, DataType type, 
            TextureFilterOption min, TextureFilterOption mag);
    void bind() const;
    void bindTexture(u32 n) const;
    void unbind() const;
};

struct MultiFrameBuffer : public FrameBuffer {
    u32 ID, depthRBO;
    std::vector<Texture> texs;

    void init();
};

struct GBuffer {
    u32 ID;
    u32 depthRBO;
    Texture pos, norm, color;

    GBuffer();
    void init(u32 width, u32 height);
    void bind() const;
    void bindTextures() const;
    void unbind() const;
};

#endif
