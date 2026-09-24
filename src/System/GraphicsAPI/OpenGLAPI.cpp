#include "GraphicsAPI.h"
#include "System/GraphicsAPI/GraphicsApi.h"
#include "global.h"
#include <GLFW/glfw3.h>

OpenGLAPI::OpenGLAPI(
        const u32 width,
        const u32 height,
        const std::string& windowName) : GraphicsAPI(width, height, windowName) 
{
    int ret = glfwInit();
    if (ret != GLFW_TRUE) {
        std::cout << "FAILED to init\n";
    }

    // Set Context version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 

    // Set to core mode
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Mac specific
    #if defined(__APPLE__)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
}

void OpenGLAPI::createWindow() {
    window = glfwCreateWindow(width, height, windowName.c_str(), NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create window\n";
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(window);

    // Initialize glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(1);
    }

    // Set viewport using the actual framebuffer size (differs from the
    // requested window size on high-DPI / Retina displays)
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);
    width = fbWidth;
    height = fbHeight;
}

void OpenGLAPI::swapBuffers() {
    glfwSwapBuffers(window);
}

bool OpenGLAPI::isWindowOpen() {
    return !glfwWindowShouldClose(window);
}

/*
 * Uniforms and Buffers
 */

u32 OpenGLAPI::createArrayBuffer(u32 size, BufferDrawType drawType, u32 index, const void* data) {
    u32 ID;
    glGenBuffers(1, &ID);
    bindArrayBuffer(ID);
    glBufferData(GL_ARRAY_BUFFER, size, data, drawType);

    return ID;
}

void OpenGLAPI::bindArrayBuffer(const u32 ID) {
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void OpenGLAPI::updateArrayBufferSubData(const u32 ID, const u32 offset, const u32 size, const void* data) {
    bindArrayBuffer(ID);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

u32 OpenGLAPI::createElementArrayBuffer(u32 size, BufferDrawType drawType, u32 index, const void* data) {
    u32 ID;
    glGenBuffers(1, &ID);
    bindElementArrayBuffer(ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, drawType);

    return ID;
}

void OpenGLAPI::bindElementArrayBuffer(const u32 ID) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void OpenGLAPI::updateElementArrayBufferSubData(const u32 ID, const u32 offset,
                                            const u32 size, const void* data) 
{
    bindElementArrayBuffer(ID);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
}

u32 OpenGLAPI::createVertexArray() {
    u32 ID;
    glGenVertexArrays(1, &ID);

    return ID;
}

void OpenGLAPI::bindVertexArray(u32 ID) {
    glBindVertexArray(ID);
}

u32 OpenGLAPI::createUniformBuffer(u32 size, BufferDrawType drawType, u32 index, const void* data) {
    u32 ID;
    glGenBuffers(1, &ID);
    glBindBuffer(GL_UNIFORM_BUFFER, ID);
    glBufferData(GL_UNIFORM_BUFFER, size, data, drawType);
    glBindBufferBase(GL_UNIFORM_BUFFER, index, ID);

    return ID;
}

void OpenGLAPI::bindUniformBuffer(u32 ID) {
    glBindBuffer(GL_UNIFORM_BUFFER, ID);
}

void OpenGLAPI::updateUniformBufferSubData(const u32 ID, const u32 offset, const u32 size, const void* data) {
    bindUniformBuffer(ID);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

u32 OpenGLAPI::createFrameBuffer() {
    u32 ID;
    glGenFramebuffers(1, &ID);

    return ID;
}

void OpenGLAPI::bindFrameBuffer(const u32 ID) {
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
}

void OpenGLAPI::unbindFrameBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLAPI::attachDepthBufferToFrameBuffer(u32 ID) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_TEXTURE_2D, ID, 0);
}

void OpenGLAPI::setVertexAttribute(u32 index, u32 size,
        DataType type, bool normalized, u32 stride,
        const void* offset) 
{
    glVertexAttribPointer(
        index, size, type,
        (normalized) ? GL_TRUE : GL_FALSE,
        stride, offset
    );
}

void OpenGLAPI::enableVertexAttributeArray(u32 index) {
    glEnableVertexAttribArray(index);
}

void OpenGLAPI::deleteBuffer(u32* ID) {
    glDeleteBuffers(1, ID);
}

void OpenGLAPI::deleteVertexArrays(u32* ID) {
    glDeleteVertexArrays(1, ID);
}

void OpenGLAPI::setDrawBuffer(FrameBufferParams param) {
    glDrawBuffer(param);
}

void OpenGLAPI::setReadBuffer(FrameBufferParams param) {
    glReadBuffer(param);
}

/*
 * Texture
 */
u32 OpenGLAPI::createTexture2D(TextureFormat internal,
                u32 width, u32 height, TextureFormat format,
                DataType type, void* data) {
    u32 ID;

    glGenTextures(1, &ID);
    bindTexture2D(ID);
    glTexImage2D(GL_TEXTURE_2D, 0, internal, width, height, 0, format, type, data);

    return ID;
}

void OpenGLAPI::bindTexture2D(u32 ID) {
    glBindTexture(GL_TEXTURE_2D, ID);
}

void OpenGLAPI::activeTexture2D(u32 texNum, u32 ID) {
    glActiveTexture(GL_TEXTURE0 + texNum);
    glBindTexture(GL_TEXTURE_2D, ID);
}

void OpenGLAPI::setTexture2DParameter(TextureFilter filter, TextureFilterOption opt) {
    glTexParameteri(GL_TEXTURE_2D, filter, opt);
}

void OpenGLAPI::setTexture2DBorderColor(Color c) {
    float col[4] = {c.r, c.g, c.b, c.a};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, col);
}

void OpenGLAPI::attachTexture2DToFramebuffer2D(u32 i, u32 ID) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, ID, 0);
}

void OpenGLAPI::generateMipMap() {
    glGenerateMipmap(GL_TEXTURE_2D); // TODO: make different ones
}

/*
 * Bools
 */
void OpenGLAPI::setDepthTest(bool val) {
    if (val) { glEnable(GL_DEPTH_TEST); }
    else { glDisable(GL_DEPTH_TEST); }
}

void OpenGLAPI::setCullFace(bool val) {
    if (val) { glEnable(GL_CULL_FACE); }
    else { glDisable(GL_CULL_FACE); }
}

void OpenGLAPI::setSRGB(bool val) {
    if (val) { glEnable(GL_FRAMEBUFFER_SRGB); }
    else { glDisable(GL_FRAMEBUFFER_SRGB); }
}

/*
 * Input
 */
Vec2 OpenGLAPI::getCursorPos() { // TODO: Make Double Vec2
    double x, y; 
    glfwGetCursorPos(window, &x, &y);

    return Vec2(x, y);
}

void OpenGLAPI::captureMouse(bool val) {
    glfwSetInputMode(window, GLFW_CURSOR, 
            (val) ? GLFW_CURSOR_DISABLED 
                    : GLFW_CURSOR_NORMAL);
}

bool OpenGLAPI::isKeyPressed(u32 keycode) {
    return glfwGetKey(window, keycode);
}

/*
 * Callbacks
 */
void OpenGLAPI::setResizeCallback(GLFWframebuffersizefun callback) {
    // Update window size with window update
    glfwSetFramebufferSizeCallback(window, callback);
}

void OpenGLAPI::setCursorPosCallback(GLFWcursorposfun callback) {
    // Update window size with window update
    glfwSetCursorPosCallback(window, callback);
}

/*
 * Drawing
 */

void OpenGLAPI::drawElements(DrawType type, u32 size, DataType dataType) {
    glDrawElements(type, size, dataType, 0);
}
