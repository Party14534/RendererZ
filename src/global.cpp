#include "global.h"

bool glInitialized = false;

void initializeGL() {
    if (glInitialized) return;
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

    glInitialized = true;
}
