#include "GBuffer.h"
#include "Objects/Texture.h"
#include "global.h"

Framebuffer::Framebuffer() { }

void Framebuffer::init(u32 width, u32 height) {
    glGenFramebuffers(1, &ID);
    glBindFramebuffer(GL_FRAMEBUFFER, ID);

    tex = Texture(RED, width, height, RED, FLOAT);
    tex.setTextureParameter(MIN_FILTER, NEAREST);
    tex.setTextureParameter(MAG_FILTER, NEAREST);
    tex.attachToFramebuffer2D(0);

    //glDrawBuffer(1);
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
    pos.setTextureParameter(WRAP_S, CLAMP_TO_EDGE);
    pos.setTextureParameter(WRAP_T, CLAMP_TO_EDGE);
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
