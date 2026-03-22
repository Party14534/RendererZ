#include "main.h"

Tri t(std::vector<float> {
        -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
});

unsigned int VBO;

void framebuffer_size_callback(GLFWwindow* win, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    glfwInit(); // Initialize GLFW

    // Set Context version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 

    // Set to core mode
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Mac specific
    #if defined(__APPLE__)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    unsigned int width = 800; 
    unsigned int height = 600;

    GLFWwindow* win = glfwCreateWindow(width, height, "Test", NULL, NULL);
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
        return -1;
    }

    // Set viewport
    glViewport(0, 0, width, height);

    // Update window size with window update
    glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);

    // Initialize vertex buffer
    glGenBuffers(1, &VBO);
    
    // Bind VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Bind the vertex buffer to the array buffer type
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Set buffer data
    glBufferData(GL_ARRAY_BUFFER, t.vertices.size() * sizeof(float), t.vertices.data(), GL_STATIC_DRAW);

    // Load in shaders
    unsigned int vertexShader = loadShader("../src/Shaders/vertex.vert", GL_VERTEX_SHADER);
    unsigned int fragmentShader = loadShader("../src/Shaders/frag.frag", GL_FRAGMENT_SHADER);

    // Link shaders to shader program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    // Now that we've linked the shaders we can delete them
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Set vertex attribute data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while(!glfwWindowShouldClose(win))
    {
        glfwSwapBuffers(win);

        // Handle rendering
        glClearColor(.88f, .76f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwPollEvents();    
    }

    glfwTerminate();
    return 0;
}
