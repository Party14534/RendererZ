#include "Window.h"
#include "Shaders/ShaderCode.h"
#include "Shaders/shaders.h"
#include "global.h"
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
    drawScreen(Drawable::Plane())
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
    this->width = fbWidth;
    this->height = fbHeight;

    // TODO: deal with resizing
    gBuffer.init(fbWidth, fbHeight);
    saoBuffer.init(RG16, fbWidth, fbHeight, RG, FLOAT, NEAREST, NEAREST);
    saoBlurHBuffer.init(RG16, fbWidth, fbHeight, RG, FLOAT, NEAREST, NEAREST);
    saoBlurBuffer.init(RG16, fbWidth, fbHeight, RG, FLOAT, NEAREST, NEAREST);

    dLightShadowBuffer.init(DEPTH, 1024 * (width / height), 1024, DEPTH, FLOAT, LINEAR, LINEAR);

    // Update window size with window update
    glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);

    gBufferShader = std::make_shared<ShaderProgram>(ShaderProgram::fromStrings(gBufferVertex, gBufferFrag));
    dLightShader = std::make_shared<ShaderProgram>(ShaderProgram::fromStrings(dLightVertex, dLightFrag));
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

    // Reserve targets beforehand to not face large penalty at launch
    renderTargets.reserve(128);

    drawScreen.setScale(Vec3(2., 2., 0.));
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

    // Shadow Map pass
    glViewport(0, 0, dLightShadowBuffer.width, dLightShadowBuffer.height);
    dLightShadowBuffer.bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    dLightShader->use();
    dLightShadowBuffer.bind();
    setDLightUniforms();
    glDisable(GL_CULL_FACE);
    for (auto& target : renderTargets) {
        target->draw(dLightShader, dLightVP);
    }
    glEnable(GL_CULL_FACE);
    dLightShadowBuffer.unbind();
    dLightShadowBuffer.bindTexture(5);


    glViewport(0, 0, width, height);
    // Draw GBuffer
    gBuffer.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gBufferShader->use();
    setGBufferUniforms();

    for (auto& target : renderTargets) {
        target->draw(gBufferShader, gBufferVP);
    }

    // Everything drawn to GBuffer, now run the light pass to the screen
    gBuffer.unbind();
    gBuffer.bindTextures();

    // SAO rendering
    saoBuffer.bind();
    glClear(GL_COLOR_BUFFER_BIT);
    saoPassShader->use();
    setSAOPassUniforms();
    glDisable(GL_CULL_FACE);
    drawScreen.drawLightPass(saoPassShader);

    saoBuffer.unbind();

    // BLUR SAO: two separable passes (horizontal then vertical) instead of
    // one 2D pass, so a wide bilateral blur stays cheap.
    saoBlurHBuffer.bind();
    saoBuffer.bindTexture(3);
    glClear(GL_COLOR_BUFFER_BIT);
    saoBlurPassShader->use();
    setSAOBlurPassUniforms(Vec2(1., 0.));
    drawScreen.drawLightPass(saoBlurPassShader);

    saoBlurHBuffer.unbind();

    saoBlurBuffer.bind();
    saoBlurHBuffer.bindTexture(3);
    glClear(GL_COLOR_BUFFER_BIT);
    saoBlurPassShader->use();
    setSAOBlurPassUniforms(Vec2(0., 1.));
    drawScreen.drawLightPass(saoBlurPassShader);

    saoBlurBuffer.unbind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    saoBlurBuffer.bindTexture(4);

    // Render lighting
    lightPassShader->use();
    setLightPassUniforms();

    // Quad is drawn in clip space directly, so its winding can be back-facing.
    drawScreen.drawLightPass(lightPassShader);
    glEnable(GL_CULL_FACE);

    renderTargets.clear();

    glfwSwapBuffers(win);
}

bool Window::isOpen() {
    return !glfwWindowShouldClose(win);
}

void Window::clear(Color c) {
    glClearColor(0., 0., 0., 1.);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //if (skyBox != nullptr) skyBox->map.bind(SKYBOX_TEXTURE_UNIT);
}

void Window::draw(IRenderable& d) {
    renderTargets.push_back(&d);
}

void Window::draw(Scene& s) {
    for (auto& target : s.sceneObjects) { 
        renderTargets.push_back(target); 
    }
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

    gBufferVP = cam.GetProjectionMatrix() * cam.GetViewMatrix();
}

void Window::setDLightUniforms() {
    calcDLightVP();
    
    dLightShader->setMat4(SHADER_LIGHT_SPACE_MATRIX_UNIFORM, dLightVP);
}

void Window::setLightPassUniforms() {
    lightPassShader->use();

    // Assign each G-buffer sampler its own texture unit, matching the units
    // bound in GBuffer::bindTextures().
    lightPassShader->setInt(SHADER_GPOSITION_UNIFORM, 0);
    lightPassShader->setInt(SHADER_GNORMAL_UNIFORM, 1);
    lightPassShader->setInt(SHADER_GALBEDO_SPEC_UNIFORM, 2);
    lightPassShader->setInt(SHADER_GSAO_BLUR_UNIFORM, 4);
    lightPassShader->setInt(SHADER_DIR_SHADOW_MAP_UNIFORM, 5);

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

    lightPassShader->setMat4(SHADER_LIGHT_SPACE_MATRIX_UNIFORM, dLightVP);

    lightPassShader->setBool(SHADER_SHOW_SAO_UNIFORM, showSao);
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

void Window::setSAOBlurPassUniforms(Vec2 direction) {
    saoBlurPassShader->use();
    saoBlurPassShader->setInt(SHADER_GSAO_UNIFORM, 3);
    saoBlurPassShader->setVec2(SHADER_SAO_BLUR_DIRECTION_UNIFORM, direction);
    saoBlurPassShader->setVec2(SHADER_RESOLUTION_UNIFORM, Vec2(width, height));
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

void Window::calcDLightVP() {
    double near = 0.1;
    double extent = 75.;
    double shadowDistance = 100.;
    double far = shadowDistance + extent;

    Vec3 target = cam.GetPos();
    Vec3 eye = target - dLight.getDir().normalize() * shadowDistance;

    Mat4D p = cam.CreateOrthographicMatrix(near, far, extent, -extent, extent, -extent);
    Mat4D v = lookAt(eye, target, Vec3(0., 1., 0.));

    dLightVP = p * v;
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

