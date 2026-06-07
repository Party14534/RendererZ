#include "Window.h"

Window::Window(u32 width, u32 height, std::string windowName) :
    width(width),
    height(height),
    windowName(windowName),
    mouse((float)width / 2.f, (float)height / 2.f)
{
    initializeGL(); // Initialize GLFW

    win = glfwCreateWindow(width, height, windowName.c_str(), NULL, NULL);
    if (win == NULL) {
        std::cout << "Failed to create window\n";
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(win);

    // Initialize glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(1);
    }

    // Set viewport using the actual framebuffer size (differs from the
    // requested window size on high-DPI / Retina displays)
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(win, &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);

    // Update window size with window update
    glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);

    objectShader = Shader("../src/Shaders/objectVertex.vert", "../src/Shaders/objectFrag.frag");
    lightShader = Shader("../src/Shaders/lightVertex.vert", "../src/Shaders/lightFrag.frag");

    // Create perspective matrices
    cam.BuildPerspectiveMatrices(width, height);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Set cursor callback
    glfwSetCursorPosCallback(win, mouseCallback);
    
    // Set mouse position
    glfwGetCursorPos(win, &mouse.x, &mouse.y);

    glEnable(GL_CULL_FACE);
}

Window::~Window() {
    glfwTerminate();
}

void Window::display() {
    glfwSwapBuffers(win);
}

bool Window::isOpen() {
    return !glfwWindowShouldClose(win);
}

void Window::clear(Color c) {
    glClearColor(c.r, c.g, c.b, c.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::draw(Drawable& d) {
    d.draw(d.isLightSource ? lightShader : objectShader,
            cam.GetViewMatrix(), cam.GetProjectionMatrix(),
            cam.GetPos());
}

/*
 * Events
 */
void Window::pollEvents() {
    glfwPollEvents();
    wasMouseMoved = false;

    if (frameCallbackFlag) {
        width = frameCallbackWidth;
        height = frameCallbackHeight;
        
        // Build perspective matrices
        cam.BuildPerspectiveMatrices(width, height);

        frameCallbackFlag = false;
    }

    if (mouseCallbackFlag) {
        // The cursor callback reports absolute positions; convert to a
        // frame-to-frame delta relative to the last position we stored.
        float dx = (float)(mouseCallbackX - mouse.x);
        float dy = (float)(mouse.y - mouseCallbackY); // screen-y grows downward; invert

        mouseChange = Vec2(dx, dy);

        mouse.x = mouseCallbackX;
        mouse.y = mouseCallbackY;

        mouseCallbackFlag = false;
        wasMouseMoved = true;
    }
}

void Window::captureMouse() {
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::uncaptureMouse() {
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

bool Window::isKeyPressed(u32 keycode) {
    return glfwGetKey(win, keycode);
}

/*
 * Callbacks
 */

void Window::mouseCallback(GLFWwindow* win, double xPos, double yPos) {
    mouseCallbackFlag = true;
    mouseCallbackX = xPos;
    mouseCallbackY = yPos;
}

void Window::framebuffer_size_callback(GLFWwindow* win, int _width, int _height) {
    glViewport(0, 0, _width, _height);
    frameCallbackFlag = true;
    frameCallbackWidth = _width;
    frameCallbackHeight = _height;
}

