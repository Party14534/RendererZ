#include "Window.h"
#include "global.h"

void framebuffer_size_callback(GLFWwindow* win, int width, int height) {
    glViewport(0, 0, width, height);
}

Window::Window(u32 width, u32 height, std::string windowName) :
    width(width),
    height(height),
    windowName(windowName),
    orthoMat(4, 4),
    perspMat(4, 4),
    viewMat(Mat::getIdentity(4))
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

    defaultShader = Shader("../src/Shaders/vertex.vert", "../src/Shaders/frag.frag");

    // Create perspective matrices
    buildPerspectiveMatrices();

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
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
    d.draw(defaultShader, viewMat, isOrthographic ? orthoMat : perspMat);
}

void Window::buildPerspectiveMatrices() {
    float near = 0.1;
    float far = 100;
    float left = 0;
    float right = width;
    float bottom = 0;
    float top = height;

    fov = 90;

    aspectRatio = (float)width / (float)height;
    orthoMat = Mat(4, 4, {
            2/(right - left), 0, 0, -((right + left) / (right - left)),
            0, 2/(top - bottom), 0, -((top + bottom) / (top - bottom)),
            0, 0, -2/(far - near), -((far + near) / (far - near)),
            0, 0, 0, 1
        }
    );

    float S = 1.f / (tan((fov * 0.5f) * (PI * (1.f / 180.f))));
    perspMat = Mat(4, 4, {
            S / aspectRatio, 0, 0, 0,
            0, S, 0, 0,
            0, 0, -((far + near) / (far - near)), -((2.f * far * near) / (far - near)),
            0, 0, -1, 0
        }
    );
}
