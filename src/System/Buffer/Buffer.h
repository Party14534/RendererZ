#ifndef BUFFER_H
#define BUFFER_H

#include "../../global.h"
#include "Math/math.h"
#include "System/GraphicsApi/GraphicsApi.h"

struct Buffer {
    u32 ID, size, index;

    ~Buffer();

    virtual void init(const u32 size, const void* data,
            const BufferDrawType drawType, const u32 index) = 0;
    virtual void bind() = 0;
    virtual void updateData(const u32 offset, const u32 size, const void* data) = 0;
    virtual void unbind() = 0;
};

struct ArrayBuffer : public Buffer {
    void init(const u32 size, const void* data,
            const BufferDrawType drawType, const u32 index);
    void bind();
    void updateData(const u32 offset, const u32 size,
            const void* data);
    void unbind();
};

struct ElementArrayBuffer : public Buffer {
    void init(const u32 size, const void* data,
            const BufferDrawType drawType, const u32 index);
    void bind();
    void updateData(const u32 offset, const u32 size,
            const void* data);
    void unbind();
};

struct UniformBuffer : public Buffer {
    void init(const u32 size, const void* data,
            const BufferDrawType drawType, const u32 index);
    void bind();
    void updateData(const u32 offset, const u32 size,
            const void* data);
    void unbind();
};

struct VertexArray {
    u32 ID;

    ~VertexArray();

    void init();
    void bind();
    void unbind();
};

#endif
