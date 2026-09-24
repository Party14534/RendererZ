#ifndef GRAPHICS_API_H
#define GRAPHICS_API_H

#include "../include/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

#include "../../Math/math.h"

enum BufferDrawType {
    DYNAMIC = GL_DYNAMIC_DRAW,
    STATIC = GL_STATIC_DRAW
};

enum DrawType {
    POINTS = GL_POINTS,
    LINE_STRIP = GL_LINE_STRIP,
    LINE_LOOP = GL_LINE_LOOP,
    LINES = GL_LINES,
    LINE_STRIP_ADJ = GL_LINE_STRIP_ADJACENCY,
    LINES_ADJ = GL_LINES_ADJACENCY,
    TRI_STRIP = GL_TRIANGLE_STRIP,
    TRI_FAN = GL_TRIANGLE_FAN,
    TRI = GL_TRIANGLES,
    TRI_STRIP_ADJ = GL_TRIANGLE_STRIP_ADJACENCY,
    TRI_ADJ = GL_TRIANGLES_ADJACENCY
};

enum FrameBufferParams {
    NONE = GL_NONE,
    FRONT_LEFT = GL_FRONT_LEFT,
    FRONT_RIGHT = GL_FRONT_RIGHT,
    BACK_LEFT = GL_BACK_LEFT,
    BACK_RIGHT = GL_BACK_RIGHT,
    FRONT = GL_FRONT, 
    BACK = GL_BACK,
    LEFT = GL_LEFT,
    RIGHT = GL_RIGHT,
    FRONT_AND_BACK = GL_FRONT_AND_BACK,
    COLOR_ATTACHMENT = GL_COLOR_ATTACHMENT0
};

enum TextureFilter {
    MIN_FILTER = GL_TEXTURE_MIN_FILTER,
    MAG_FILTER = GL_TEXTURE_MAG_FILTER,
    WRAP_S = GL_TEXTURE_WRAP_S,
    WRAP_T = GL_TEXTURE_WRAP_T,
    WRAP_R = GL_TEXTURE_WRAP_R,
    COMPARE_MODE = GL_TEXTURE_COMPARE_MODE
};

enum TextureFilterOption {
    NEAREST = GL_NEAREST,
    LINEAR = GL_LINEAR,
    REPEAT = GL_REPEAT,
    MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
    CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
    CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
    REF_TO_TEX = GL_COMPARE_REF_TO_TEXTURE
};

enum TextureFormat {
    RED = GL_RED,
    RG = GL_RG,
    SRGB = GL_SRGB,
    RGB = GL_RGB,
    RGBA = GL_RGBA,
    RG16 = GL_RG16F,
    RGBA16 = GL_RGBA16F,
    DEPTH = GL_DEPTH_COMPONENT
};

enum MipMapFilterOption {
    NEAREST_NEAREST,
    LINEAR_NEAREST,
    NEAREST_LINEAR,
    LINEAR_LINEAR
};

enum DataType {
    FLOAT = GL_FLOAT,
    UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
    UNSIGNED_INT = GL_UNSIGNED_INT
};

struct GraphicsAPI {
    public:
        u32 width, height;
        std::string windowName;

        GraphicsAPI(const u32 width, const u32 height, const std::string& windowName);

        virtual void createWindow() = 0;

        // Uniforms and Buffers
        virtual u32 createUniformBuffer(u32 size, BufferDrawType drawType, u32 index, const void* data) = 0;
        virtual void bindUniformBuffer(const u32 ID) = 0;
        virtual void updateUniformBufferSubData(const u32 ID, const u32 offset, const u32 size, const void* data) = 0;

        virtual u32 createArrayBuffer(u32 size, BufferDrawType drawType, u32 index, const void* data) = 0;
        virtual void bindArrayBuffer(const u32 ID) = 0;
        virtual void updateArrayBufferSubData(const u32 ID, const u32 offset, const u32 size, const void* data) = 0;

        virtual u32 createElementArrayBuffer(u32 size, BufferDrawType drawType, u32 index, const void* data) = 0;
        virtual void bindElementArrayBuffer(const u32 ID) = 0;
        virtual void updateElementArrayBufferSubData(const u32 ID, const u32 offset, const u32 size, const void* data) = 0;

        virtual u32 createVertexArray() = 0;
        virtual void bindVertexArray(u32 ID) = 0;

        virtual u32 createFrameBuffer() = 0;
        virtual void bindFrameBuffer(const u32 ID) = 0;
        virtual void unbindFrameBuffer() = 0;
        virtual void attachDepthBufferToFrameBuffer(u32 ID) = 0;

        virtual void setVertexAttribute(u32 index, u32 size,
                DataType type, bool normalized, u32 stride,
                const void* offset) = 0;
        virtual void enableVertexAttributeArray(u32 index) = 0;

        virtual void deleteBuffer(u32* ID) = 0;
        virtual void deleteVertexArrays(u32* ID) = 0;

        // Texture
        virtual u32 createTexture2D(TextureFormat internal,
                u32 width, u32 height, TextureFormat format,
                DataType type, void* data) = 0;
        virtual void bindTexture2D(u32 ID) = 0;
        virtual void activeTexture2D(u32 texNum, u32 ID) = 0;
        virtual void setTexture2DParameter(TextureFilter filter, TextureFilterOption opt) = 0;
        virtual void setTexture2DBorderColor(Color c) = 0;
        virtual void attachTexture2DToFramebuffer2D(u32 index, u32 texID) = 0;
        virtual void generateMipMap() = 0;

        // Bool
        virtual void setDepthTest(bool val) = 0;
        virtual void setCullFace(bool val) = 0;
        virtual void setSRGB(bool val) = 0;
        virtual void setDrawBuffer(FrameBufferParams param) = 0;
        virtual void setReadBuffer(FrameBufferParams param) = 0;

        // Input
        virtual Vec2 getCursorPos() = 0;
        virtual void captureMouse(bool val) = 0;
        virtual bool isKeyPressed(u32 keycode) = 0;

        // Callbacks
        virtual void setResizeCallback(GLFWframebuffersizefun callback) = 0;
        virtual void setCursorPosCallback(GLFWcursorposfun callback) = 0;

        // Window Management
        virtual void swapBuffers() = 0;
        virtual bool isWindowOpen() = 0;

        // Drawing
        virtual void drawElements(DrawType type, u32 size, DataType dataType) = 0;
};

struct OpenGLAPI : public GraphicsAPI {
    public:
        OpenGLAPI(const u32 width, const u32 height, const std::string& windowName);

        void createWindow();

        // Uniform and Buffers
        u32 createUniformBuffer(u32 size, BufferDrawType drawType, u32 index, const void* data);
        void bindUniformBuffer(const u32 ID);
        void updateUniformBufferSubData(const u32 ID, const u32 offset, const u32 size, const void* data);

        u32 createArrayBuffer(u32 size, BufferDrawType drawType, u32 index, const void* data);
        void bindArrayBuffer(const u32 ID);
        void updateArrayBufferSubData(const u32 ID, const u32 offset, const u32 size, const void* data);

        u32 createElementArrayBuffer(u32 size, BufferDrawType drawType, u32 index, const void* data);
        void bindElementArrayBuffer(const u32 ID);
        void updateElementArrayBufferSubData(const u32 ID, const u32 offset, const u32 size, const void* data);

        u32 createVertexArray();
        void bindVertexArray(u32 ID);

        u32 createFrameBuffer();
        void bindFrameBuffer(const u32 ID);
        void unbindFrameBuffer();
        void attachDepthBufferToFrameBuffer(u32 ID);

        void setVertexAttribute(u32 index, u32 size,
                DataType type, bool normalized, u32 stride,
                const void* offset);
        void enableVertexAttributeArray(u32 index);

        void deleteBuffer(u32* ID);
        void deleteVertexArrays(u32* ID);

        // Texture
        u32 createTexture2D(TextureFormat internal,
                u32 width, u32 height, TextureFormat format,
                DataType type, void* data);
        void bindTexture2D(u32 ID);
        void activeTexture2D(u32 texNum, u32 ID);
        void setTexture2DParameter(TextureFilter filter, TextureFilterOption opt);
        void setTexture2DBorderColor(Color c);
        void attachTexture2DToFramebuffer2D(u32 index, u32 texID);
        void generateMipMap();

        // Bool
        void setDepthTest(bool val);
        void setCullFace(bool val);
        void setSRGB(bool val);
        void setDrawBuffer(FrameBufferParams param);
        void setReadBuffer(FrameBufferParams param);

        // Input
        Vec2 getCursorPos();
        void captureMouse(bool val);
        bool isKeyPressed(u32 keycode);

        // Callbacks
        void setResizeCallback(GLFWframebuffersizefun callback);
        void setCursorPosCallback(GLFWcursorposfun callback);

        // Window Management
        void swapBuffers();
        bool isWindowOpen();

        // Drawing
        void drawElements(DrawType type, u32 size, DataType dataType);

    private:
        GLFWwindow* window;
};

#endif
