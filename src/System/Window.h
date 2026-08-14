#ifndef WINDOW_H
#define WINDOW_H

#include <list>
#include <string>
#include <vector>

#include "../Objects/Drawable.h"
#include "../Math/math.h"
#include "../Shaders/shaders.h"
#include "Camera.h"
#include "../Input/Input.h"
#include "../Objects/LightSource.h"
#include "../Objects/SkyBox.h"
#include "../Objects/Scene.h"
#include "System/GBuffer.h"

struct Window {
    // Window variables
    u32 width, height;
    std::string windowName;

    std::shared_ptr<ShaderProgram> gBufferShader = nullptr;
    std::shared_ptr<ShaderProgram> lightPassShader = nullptr;
    std::shared_ptr<ShaderProgram> saoPassShader = nullptr;
    std::shared_ptr<ShaderProgram> saoBlurPassShader = nullptr;

    GBuffer gBuffer;
    Framebuffer saoBuffer;
    Framebuffer saoBlurBuffer;
    Drawable lightPassScreen;
    Drawable saoPassScreen;
    Drawable saoBlurPassScreen;
    u32 pointLightUBO;

    std::shared_ptr<SkyBox> skyBox = nullptr;

    Camera cam;
    Mouse mouse;

    // Lighting
    DirLight dLight;
    std::vector<std::shared_ptr<PointLight>> pLights;

    bool wasMouseMoved;
    Vec2 mouseChange;

    Window(u32 width, u32 height, std::string windowName);
    ~Window();

    void display();
    bool isOpen();
    void clear(Color c);
    void draw(Drawable& d);
    void draw(ComplexDrawable& d);
    void draw(Scene& scene);

    void setSkyBox(std::shared_ptr<SkyBox> skyBox);
    
    // Uniforms
    void setGBufferUniforms();
    void setLightPassUniforms();
    void setSAOPassUniforms();
    void setSAOBlurPassUniforms();
    void setPointLightUniforms();

    // Events
    void pollEvents();
    void captureMouse();
    void uncaptureMouse();
    bool isKeyPressed(u32 keycode);
    void createKeyReleaseCallback(u32 keycode);
    Vec2 GetMouseChange();

    // Lighting
    void addPointLight(PointLight p);
    void removePointLight(u32 i);

    // Callbacks
    static void mouseCallback(GLFWwindow* win, double xPos, double yPos);
    static void framebuffer_size_callback(GLFWwindow* win, int _width, int _height);
};

#endif
