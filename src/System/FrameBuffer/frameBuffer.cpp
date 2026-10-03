#include "FrameBuffer.h"
#include "System/GraphicsAPI/GraphicsApi.h"
#include "global.h"

void FrameBuffer::init(TextureFormat internal, u32 width, u32 height,
        TextureFormat format, DataType type, 
        TextureFilterOption min, TextureFilterOption mag)
{
    this->width = width;
    this->height = height;

    ID = api->createFrameBuffer();
    api->bindFrameBuffer(ID);

    tex = Texture(internal, width, height, format, type, (void*)(0));
    tex.setTextureParameter(MIN_FILTER, min);
    tex.setTextureParameter(MAG_FILTER, mag);

    if (internal == DEPTH) {
        tex.setTextureParameter(COMPARE_MODE, REF_TO_TEX);
        tex.setTextureParameter(WRAP_S, CLAMP_TO_BORDER);
        tex.setTextureParameter(WRAP_T, CLAMP_TO_BORDER);
        tex.setTextureBorderColor(Color(1.));
        api->attachDepthBufferToFrameBuffer(tex.ID);
        api->setDrawBuffer(NONE);
        api->setReadBuffer(NONE);
    } else {
        tex.attachToFramebuffer2D(0);
    }
}

void FrameBuffer::bind() const {
    api->bindFrameBuffer(ID);
}

void FrameBuffer::bindTexture(u32 n) const {
    tex.setActive(n);
}

void FrameBuffer::unbind() const {
    api->unbindFrameBuffer();
}

GBuffer::GBuffer() { }

void GBuffer::init(u32 width, u32 height) {
    glGenFramebuffers(1, &ID);
    glBindFramebuffer(GL_FRAMEBUFFER, ID);

    pos = Texture(RGBA16, width, height, RGBA, FLOAT, nullptr);
    pos.setTextureParameter(MIN_FILTER, NEAREST);
    pos.setTextureParameter(MAG_FILTER, NEAREST);
    pos.setTextureParameter(WRAP_S, MIRRORED_REPEAT);
    pos.setTextureParameter(WRAP_T, MIRRORED_REPEAT);
    pos.attachToFramebuffer2D(0);

    norm = Texture(RGBA16, width, height, RGBA, FLOAT, nullptr);
    norm.setTextureParameter(MIN_FILTER, NEAREST);
    norm.setTextureParameter(MAG_FILTER, NEAREST);
    norm.attachToFramebuffer2D(1);

    color = Texture(RGBA, width, height, RGBA, UNSIGNED_BYTE, nullptr);
    color.setTextureParameter(MIN_FILTER, NEAREST);
    color.setTextureParameter(MAG_FILTER, NEAREST);
    color.attachToFramebuffer2D(2);

    u32 attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);

    glGenRenderbuffers(1, &depthRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRBO);
}

void GBuffer::bind() const {
    api->bindFrameBuffer(ID);
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
}

void GBuffer::bindTextures() const {
    pos.setActive(0);
    norm.setActive(1);
    color.setActive(2);
}

void GBuffer::unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
