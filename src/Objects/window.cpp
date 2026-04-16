#include "Window.h"

void framebuffer_size_callback(GLFWwindow* win, int width, int height) {
    glViewport(0, 0, width, height);
}

Window::Window(u32 width, u32 height, std::string windowName) :
    width(width),
    height(height),
    windowName(windowName)
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

    // Set viewport
    glViewport(0, 0, width, height);

    // Update window size with window update
    glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);

    defaultShader = Shader("../src/Shaders/vertex.vert", "../src/Shaders/frag.frag");
}

Window::~Window() {
    glfwTerminate();
}

bool Window::isOpen() {
    glfwSwapBuffers(win);
    return !glfwWindowShouldClose(win);
}

void Window::clear(Color c) {
    glClearColor(.88f, .76f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::draw(Drawable& d) {
    d.draw(defaultShader);
}

