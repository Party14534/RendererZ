#include "Window.h"
#include "Shaders/ShaderCode.h"
#include "Shaders/shaders.h"
#include <vector>

// Mirrors PointLightBlock's std140 layout in lightPassFrag.frag.
struct GPUPointLight {
    float pos[4];
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float attenuation[4];
};

static void writeVec3(float* dst, const Vec3& v) {
    dst[0] = v.x; dst[1] = v.y; dst[2] = v.z; dst[3] = 0.f;
}

Window::Window(u32 width, u32 height, std::string windowName) :
    width(width),
    height(height),
    windowName(windowName),
    mouse((float)width / 2.f, (float)height / 2.f),
    dLight(Vec3(0., -1, 0.), DirLightProperties()),
    lightPassScreen(Drawable::Plane()),
    saoPassScreen(Drawable::Plane()),
    saoBlurPassScreen(Drawable::Plane())
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

    gBuffer.init(fbWidth, fbHeight);
    saoBuffer.init(fbWidth, fbHeight);
    saoBlurBuffer.init(fbWidth, fbHeight);

    // Update window size with window update
    glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);

    gBufferShader = std::make_shared<ShaderProgram>(ShaderProgram::fromStrings(gBufferVertex, gBufferFrag));
    lightPassShader = std::make_shared<ShaderProgram>(ShaderProgram::fromStrings(lightPassVertex, lightPassFrag));
    saoPassShader = std::make_shared<ShaderProgram>(ShaderProgram::fromStrings(lightPassVertex, saoPassFrag));
    saoBlurPassShader = std::make_shared<ShaderProgram>(ShaderProgram::fromStrings(lightPassVertex, saoBlurPassFrag));

    glGenBuffers(1, &pointLightUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, pointLightUBO);
    glBufferData(GL_UNIFORM_BUFFER, MAX_POINT_LIGHTS * sizeof(GPUPointLight), NULL, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, POINT_LIGHT_UBO_BINDING, pointLightUBO);
    lightPassShader->bindUniformBlock(SHADER_POINT_LIGHT_BLOCK, POINT_LIGHT_UBO_BINDING);

    // gl_PointSize in the vertex shader is ignored unless this is enabled.
    /* TODO: POINTS
    glEnable(GL_PROGRAM_POINT_SIZE);
    pointShader->use();
    pointShader->setFloat(SHADER_POINT_SIZE_UNIFORM, 8.f);*/

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

    lightPassScreen.setScale(Vec3(2., 2., 0.));
    saoPassScreen.setScale(Vec3(2., 2., 0.));
    saoBlurPassScreen.setScale(Vec3(2., 2., 0.));
}

Window::~Window() {
    glfwTerminate();
}

void Window::display() {
    // If a sky box is set draw that now
    /*if (skyBox != nullptr) {
        Mat view = cam.GetViewMatrix().scaleDown().scaleUp();
        view.set(3, 3, 1.f);
        glDepthFunc(GL_LEQUAL);
        skyBoxShader->use(); 
        skyBoxShader->setMat4(SHADER_VIEW_SET_UNIFORM, view);
        skyBox->draw(skyBoxShader);
        glDepthFunc(GL_LESS);
    }*/

    // Everything drawn to GBuffer, now run the light pass to the screen
    gBuffer.unbind();
    gBuffer.bindTextures();

    // SAO rendering
    saoBuffer.bind();
    glClear(GL_COLOR_BUFFER_BIT);
    saoPassShader->use();
    setSAOPassUniforms();
    glDisable(GL_CULL_FACE);
    saoPassScreen.drawLightPass(saoPassShader);

    saoBuffer.unbind();

    // BLUR SAO
    saoBlurBuffer.bind();
    saoBuffer.bindTexture(3);
    glClear(GL_COLOR_BUFFER_BIT);
    saoBlurPassShader->use();
    setSAOBlurPassUniforms();
    saoBlurPassScreen.drawLightPass(saoBlurPassShader);

    saoBlurBuffer.unbind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    saoBlurBuffer.bindTexture(4);

    // Render lighting
    lightPassShader->use();
    setLightPassUniforms();

    // Quad is drawn in clip space directly, so its winding can be back-facing.
    lightPassScreen.drawLightPass(lightPassShader);
    glEnable(GL_CULL_FACE);

    glfwSwapBuffers(win);
}

bool Window::isOpen() {
    return !glfwWindowShouldClose(win);
}

void Window::clear(Color c) {
    gBuffer.bind();
    glClearColor(0., 0., 0., 1.);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /*
    setDefaultUniforms(objectShader);
    setDefaultUniforms(skyBoxShader);
    setDefaultUniforms(pointShader);
    if (skyBox != nullptr) skyBox->map.bind(SKYBOX_TEXTURE_UNIT);*/
    gBufferShader->use();
    setGBufferUniforms();
}

void Window::draw(Drawable& d) {
    d.draw(gBufferShader);
}

void Window::draw(ComplexDrawable& d) {
    d.draw(gBufferShader);
}

void Window::draw(Scene& s) {
    s.draw(gBufferShader);
}

void Window::setSkyBox(std::shared_ptr<SkyBox> _skyBox) {
    skyBox = _skyBox;
}

/*
 * Uniforms
 */
void Window::setGBufferUniforms() {
    // Assign each sampler its own texture unit. Without this both default to
    // unit 0, which is illegal for differing sampler types and triggers 1282.
    gBufferShader->setInt(SHADER_TEX_UNIFORM, 0);
    gBufferShader->setInt(SHADER_NORMAL_MAP_UNIFORM, 1);
    gBufferShader->setInt(SHADER_SKYBOX_UNIFORM, SKYBOX_TEXTURE_UNIT);

    gBufferShader->setMat4(SHADER_VIEW_SET_UNIFORM, cam.GetViewMatrix());
    gBufferShader->setMat4(SHADER_PROJECTION_SET_UNIFORM, cam.GetProjectionMatrix());
}

void Window::setLightPassUniforms() {
    lightPassShader->use();

    // Assign each G-buffer sampler its own texture unit, matching the units
    // bound in GBuffer::bindTextures().
    lightPassShader->setInt(SHADER_GPOSITION_UNIFORM, 0);
    lightPassShader->setInt(SHADER_GNORMAL_UNIFORM, 1);
    lightPassShader->setInt(SHADER_GALBEDO_SPEC_UNIFORM, 2);
    lightPassShader->setInt(SHADER_GSAO_BLUR_UNIFORM, 4);

    setPointLightUniforms();

    Vec3 lc = Vec3(dLight.getColor().toRGB());
    lightPassShader->setDirLight(dLight.getDir(), 
                lc * dLight.properties.ambient,
                lc * dLight.properties.diffuse,
                lc * dLight.properties.specular);

    //lightPassShader->setBool(SHADER_SKYBOX_SET_UNIFORM, skyBox != nullptr);

    lightPassShader->setInt(SHADER_POINT_LIGHT_COUNT, pLights.size());

    lightPassShader->setVec3(SHADER_VIEW_POSITION_UNIFORM, cam.GetPos());

    lightPassShader->setMat4(SHADER_VIEW_SET_UNIFORM, cam.GetViewMatrix());
    lightPassShader->setMat4(SHADER_PROJECTION_SET_UNIFORM, cam.GetProjectionMatrix());
}

void Window::setSAOPassUniforms() {
    saoPassShader->use();

    // Assign each G-buffer sampler its own texture unit, matching the units
    // bound in GBuffer::bindTextures().
    saoPassShader->setInt(SHADER_GPOSITION_UNIFORM, 0);
    saoPassShader->setInt(SHADER_GNORMAL_UNIFORM, 1);
    saoPassShader->setInt(SHADER_GALBEDO_SPEC_UNIFORM, 2);

    saoPassShader->setVec3(SHADER_VIEW_POSITION_UNIFORM, cam.GetPos());

    saoPassShader->setVec2(SHADER_RESOLUTION_UNIFORM, Vec2(width, height));

    saoPassShader->setMat4(SHADER_VIEW_SET_UNIFORM, cam.GetViewMatrix());
    saoPassShader->setMat4(SHADER_PROJECTION_SET_UNIFORM, cam.GetProjectionMatrix());
}

void Window::setSAOBlurPassUniforms() {
    saoBlurPassShader->use();
    saoBlurPassShader->setInt(SHADER_GSAO_UNIFORM, 3);
}

void Window::setPointLightUniforms() {
    u32 count = pLights.size() < MAX_POINT_LIGHTS ? pLights.size() : MAX_POINT_LIGHTS;

    std::vector<GPUPointLight> data(count);
    for (u32 i = 0; i < count; i++) {
        const std::shared_ptr<PointLight>& l = pLights[i];
        Vec3 lc = Vec3(l->getColor().toRGB());
        writeVec3(data[i].pos, l->getPos());
        writeVec3(data[i].ambient, lc * l->properties.ambient);
        writeVec3(data[i].diffuse, lc * l->properties.diffuse);
        writeVec3(data[i].specular, lc * l->properties.specular);
        writeVec3(data[i].attenuation, l->properties.attenuation);
    }

    glBindBuffer(GL_UNIFORM_BUFFER, pointLightUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, count * sizeof(GPUPointLight), data.data());
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

        // TODO: rebuild gBuffer

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

