#ifndef WINDOW_H
#define WINDOW_H

#include <string>

#include "../Objects/Drawable.h"
#include "../Math/math.h"
#include "../Shaders/shaders.h"
#include "Camera.h"
#include "../Input/Input.h"

struct Window {
    // Window variables
    u32 width, height;
    std::string windowName;

    Shader defaultShader;

    Camera cam;
    Mouse mouse;

    bool wasMouseMoved;
    Vec2 mouseChange;

    Window(u32 width, u32 height, std::string windowName);
    ~Window();

    void display();
    bool isOpen();
    void clear(Color c);
    void draw(Drawable& d);

    // Events
    void pollEvents();
    void captureMouse();
    void uncaptureMouse();
    bool isKeyPressed(u32 keycode);
    void createKeyReleaseCallback(u32 keycode);
    Vec2 GetMouseChange();

    // Callbacks
    static void mouseCallback(GLFWwindow* win, double xPos, double yPos);
    static void framebuffer_size_callback(GLFWwindow* win, int _width, int _height);
};

#endif
