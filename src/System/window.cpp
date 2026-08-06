#include "Window.h"
#include "Shaders/ShaderCode.h"
#include <vector>

Window::Window(u32 width, u32 height, std::string windowName) :
    width(width),
    height(height),
    windowName(windowName),
    mouse((float)width / 2.f, (float)height / 2.f),
    dLight(Vec3(0., -1, 0.), DirLightProperties())
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

    objectShader = std::make_shared<Shader>(Shader::fromStrings(objectVertex, objectFrag));
    lightShader = std::make_shared<Shader>(Shader::fromStrings(lightVertex, lightFrag));
    skyBoxShader = std::make_shared<Shader>(Shader::fromStrings(skyBoxVertex, skyBoxFrag));
    pointShader = std::make_shared<Shader>(Shader::fromStrings(pointVertex, pointFrag));

    // gl_PointSize in the vertex shader is ignored unless this is enabled.
    glEnable(GL_PROGRAM_POINT_SIZE);
    pointShader->use();
    pointShader->setFloat(SHADER_POINT_SIZE_UNIFORM, 8.f);

    // Create perspective matrices
    cam.BuildPerspectiveMatrices(width, height);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Set cursor callback
    glfwSetCursorPosCallback(win, mouseCallback);
    
    // Set mouse position
    glfwGetCursorPos(win, &mouse.x, &mouse.y);

    glEnable(GL_CULL_FACE);
    glEnable(GL_FRAMEBUFFER_SRGB);

    // Initialize Lights
    pLights = std::vector<std::shared_ptr<PointLight>>(0);
}

Window::~Window() {
    glfwTerminate();
}

void Window::display() {
    // If a sky box is set draw that now
    if (skyBox != nullptr) {
        Mat view = cam.GetViewMatrix().scaleDown().scaleUp();
        view.set(3, 3, 1.f);
        glDepthFunc(GL_LEQUAL);
        skyBoxShader->use(); 
        skyBoxShader->setMat4(SHADER_VIEW_SET_UNIFORM, view);
        skyBox->draw(skyBoxShader);
        glDepthFunc(GL_LESS);
    }

    glfwSwapBuffers(win);
}

bool Window::isOpen() {
    return !glfwWindowShouldClose(win);
}

void Window::clear(Color c) {
    glClearColor(c.r, c.g, c.b, c.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setDefaultUniforms(objectShader);
    setDefaultUniforms(skyBoxShader);
    setDefaultUniforms(pointShader);
    if (skyBox != nullptr) skyBox->map.bind(SKYBOX_TEXTURE_UNIT);
}

void Window::draw(Drawable& d) {
    d.draw(objectShader);
}

void Window::draw(ComplexDrawable& d) {
    d.draw(objectShader);
}

void Window::draw(Scene& s) {
    s.draw(objectShader);
}

void Window::setSkyBox(std::shared_ptr<SkyBox> _skyBox) {
    skyBox = _skyBox;
}

/*
 * Uniforms
 */
void Window::setDefaultUniforms(std::shared_ptr<Shader> shader) {
    shader->use();
    setPointLightUniforms(shader);

    Vec3 lc = Vec3(dLight.getColor().toRGB());
    shader->setDirLight(dLight.getDir(), 
                lc * dLight.properties.ambient,
                lc * dLight.properties.diffuse,
                lc * dLight.properties.specular);

    shader->setBool(SHADER_SKYBOX_SET_UNIFORM, skyBox != nullptr);

    // Assign each sampler its own texture unit. Without this both default to
    // unit 0, which is illegal for differing sampler types and triggers 1282.
    shader->setInt(SHADER_TEX_UNIFORM, 0);
    shader->setInt(SHADER_SKYBOX_UNIFORM, SKYBOX_TEXTURE_UNIT);

    shader->setInt(SHADER_POINT_LIGHT_COUNT, pLights.size());

    shader->setVec3(SHADER_VIEW_POSITION_UNIFORM, cam.GetPos());

    shader->setMat4(SHADER_VIEW_SET_UNIFORM, cam.GetViewMatrix());
    shader->setMat4(SHADER_PROJECTION_SET_UNIFORM, cam.GetProjectionMatrix());
}

void Window::setPointLightUniforms(std::shared_ptr<Shader> shader) {
    for (u32 i = 0; i < pLights.size(); i++) {
        const std::shared_ptr<PointLight> l = pLights[i];
        Vec3 lc = Vec3(l->getColor().toRGB());
        shader->setPointLight(
                GetPointLightName(i),
                l->getPos(),
                lc * l->properties.ambient,
                lc * l->properties.diffuse,
                lc * l->properties.specular,
                l->properties.attenuation);
    }
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
 * Lighting
 */
void Window::addPointLight(PointLight p) {
    pLights.emplace_back(std::make_shared<PointLight>(p));
    // TODO: Update shader stuff
}

void Window::removePointLight(u32 i) {
    pLights.erase(pLights.begin() + i);
    // TODO: Update shader stuff
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

