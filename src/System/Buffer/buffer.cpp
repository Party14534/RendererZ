#include "Buffer.h"

Buffer::~Buffer() {
    api->deleteBuffer(&ID);
}

/*
 * ArrayBuffer
 */
void ArrayBuffer::init(const u32 size, const void* data, const BufferDrawType drawType, const u32 index) {
    ID = api->createArrayBuffer(size, drawType, index, data);
    this->index = index;
}

void ArrayBuffer::bind() {
    api->bindArrayBuffer(ID);
}

void ArrayBuffer::unbind() {
    api->bindArrayBuffer(0);
}

void ArrayBuffer::updateData(const u32 offset, const u32 size, const void* data) {
    api->updateArrayBufferSubData(ID, offset, size, data);
}

/*
 * ElementArrayBuffer
 */
void ElementArrayBuffer::init(const u32 size, const void* data, const BufferDrawType drawType, const u32 index) {
    ID = api->createElementArrayBuffer(size, drawType, index, data);
    this->index = index;
}

void ElementArrayBuffer::bind() {
    api->bindElementArrayBuffer(ID);
}

void ElementArrayBuffer::unbind() {
    api->bindElementArrayBuffer(0);
}

void ElementArrayBuffer::updateData(const u32 offset, const u32 size, const void* data) {
    api->updateElementArrayBufferSubData(ID, offset, size, data);
}

/*
 * Uniform Buffer
 */
void UniformBuffer::init(const u32 size, const void* data, const BufferDrawType drawType, const u32 index) {
    ID = api->createUniformBuffer(size, drawType, index, data);
    this->index = index;
}

void UniformBuffer::bind() {
    api->bindUniformBuffer(ID);
}

void UniformBuffer::unbind() {
    api->bindUniformBuffer(0);
}

void UniformBuffer::updateData(const u32 offset, const u32 size, const void* data) {
    api->updateUniformBufferSubData(ID, offset, size, data);
}

/*
 * Vertex Array
 */
VertexArray::~VertexArray() { api->deleteVertexArrays(&ID); }

void VertexArray::init() {
    ID = api->createVertexArray(); 
}

void VertexArray::bind() {
    api->bindVertexArray(ID);
}

void VertexArray::unbind() {
    api->bindVertexArray(0);
}
