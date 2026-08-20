#include "GBuffer.h"
#include "Objects/Texture.h"
#include "Shaders/shaders.h"
#include "global.h"

Framebuffer::Framebuffer() { }

void Framebuffer::init(TextureFormat internal, u32 width, u32 height,
        TextureFormat format, TexturePixelDataType type, 
        TextureFilterOption min, TextureFilterOption mag)
{
    this->width = width;
    this->height = height;

    glGenFramebuffers(1, &ID);
    glBindFramebuffer(GL_FRAMEBUFFER, ID);

    tex = Texture(internal, width, height, format, type);
    tex.setTextureParameter(MIN_FILTER, min);
    tex.setTextureParameter(MAG_FILTER, mag);

    if (internal == DEPTH) {
        tex.setTextureParameter(COMPARE_MODE, REF_TO_TEX);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                GL_TEXTURE_2D, tex.ID, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    } else {
        tex.attachToFramebuffer2D(0);
    }
}

void Framebuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
}

void Framebuffer::bindTexture(u32 n) {
    tex.setActive(n);
}

void Framebuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GBuffer::GBuffer() { }

void GBuffer::init(u32 width, u32 height) {
    glGenFramebuffers(1, &ID);
    glBindFramebuffer(GL_FRAMEBUFFER, ID);

    pos = Texture(RGBA16, width, height, RGBA, FLOAT);
    pos.setTextureParameter(MIN_FILTER, NEAREST);
    pos.setTextureParameter(MAG_FILTER, NEAREST);
    pos.setTextureParameter(WRAP_S, MIRRORED_REPEAT);
    pos.setTextureParameter(WRAP_T, MIRRORED_REPEAT);
    pos.attachToFramebuffer2D(0);

    norm = Texture(RGBA16, width, height, RGBA, FLOAT);
    norm.setTextureParameter(MIN_FILTER, NEAREST);
    norm.setTextureParameter(MAG_FILTER, NEAREST);
    norm.attachToFramebuffer2D(1);

    color = Texture(RGBA, width, height, RGBA, UNSIGNED_BYTE);
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

void GBuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
}

void GBuffer::bindTextures() {
    pos.setActive(0);
    norm.setActive(1);
    color.setActive(2);
}

void GBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
