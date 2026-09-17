#include "Window.h"
#include "Shaders/ShaderCode.h"
#include "Shaders/shaders.h"
#include "System/GraphicsAPI/GraphicsPipeline.h"
#include "System/GraphicsApi/GraphicsApi.h"
#include <cmath>
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

Window::Window(u32 _width, u32 _height, std::string windowName) :
    width(_width),
    height(_height),
    windowName(windowName),
    mouse((float)_width / 2.f, (float)_height / 2.f),
    dLight(Vec3(0., -1, 0.), DirLightProperties()),
    drawScreen(Drawable::Plane()),
    _api(std::make_shared<OpenGLAPI>(_width, _height, windowName))
{
    api = _api;
    api->createWindow();
    width = api->width;
    height = api->height;

    gBuffer.init(api->width, api->height);
    saoBuffer.init(RG16, api->width, api->height, RG, FLOAT, NEAREST, NEAREST);
    saoBlurHBuffer.init(RG16, api->width, api->height, RG, FLOAT, NEAREST, NEAREST);
    saoBlurBuffer.init(RG16, api->width, api->height, RG, FLOAT, NEAREST, NEAREST);

    int dBufferWidth = int(1024.f * (float(width) / float(height)));

    dLightShadowBuffer.init(DEPTH, dBufferWidth, 1024, DEPTH, FLOAT, LINEAR, LINEAR);

    gBufferShader = std::make_shared<ShaderProgram>(ShaderProgram::fromStrings(gBufferVertex, gBufferFrag));
    dLightShader = std::make_shared<ShaderProgram>(ShaderProgram::fromStrings(dLightVertex, dLightFrag));
    lightPassShader = std::make_shared<ShaderProgram>(ShaderProgram::fromStrings(lightPassVertex, lightPassFrag));
    saoPassShader = std::make_shared<ShaderProgram>(ShaderProgram::fromStrings(lightPassVertex, saoPassFrag));
    saoBlurPassShader = std::make_shared<ShaderProgram>(ShaderProgram::fromStrings(lightPassVertex, saoBlurPassFrag));


    pointLightUBO.init(MAX_POINT_LIGHTS * sizeof(GPUPointLight), 0, DYNAMIC, 0);
    lightPassShader->bindUniformBlock(SHADER_POINT_LIGHT_BLOCK, pointLightUBO);

    // gl_PointSize in the vertex shader is ignored unless this is enabled.
    /* TODO: POINTS
    glEnable(GL_PROGRAM_POINT_SIZE);
    pointShader->use();
    pointShader->setFloat(SHADER_POINT_SIZE_UNIFORM, 8.f);*/

    // Create perspective matrices
    cam.BuildPerspectiveMatrices(width, height);

    // Set callbacks
    api->setResizeCallback(framebuffer_size_callback);
    api->setCursorPosCallback(mouseCallback);
    
    // Set mouse position
    mouse.pos = api->getCursorPos();

    // Enable depth testing
    api->setDepthTest(true);
    api->setSRGB(true);
    api->setCullFace(true);

    // Initialize Lights
    pLights = std::vector<std::shared_ptr<PointLight>>(0);

    // Reserve targets beforehand to not face large penalty at launch
    renderTargets.reserve(128);

    drawScreen.setScale(Vec3(2., 2., 0.));
}

Window::~Window() {
    glfwTerminate();
}

void testCallback() {
    api->setCullFace(false);
}

void Window::display(GraphicsPipeline& pipeline) {
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

    pipeline.runPipeline(renderTargets);
    renderTargets.clear();
    api->swapBuffers();
}

bool Window::isOpen() {
    return api->isWindowOpen();
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

    pointLightUBO.bind();
    glBufferSubData(GL_UNIFORM_BUFFER, 0, count * sizeof(GPUPointLight), data.data());
}

void Window::calcDLightVP() {
    double near = 0.1;
    double extent = 16.;
    double shadowDistance = 10000.;
    double far = shadowDistance + extent;

    Vec3 lightDir = dLight.getDir().normalize();
    Vec3 up(0., 1., 0.);

    // Basis matching lookAt()'s internal axes, so the snap below happens
    // in the same space as the shadow map's texel grid.
    Vec3 direction = lightDir * -1.f;
    Vec3 right = up.cross(direction).normalize();
    Vec3 camUp = direction.cross(right);

    // Snap the shadow camera's position to whole shadow-map texels so it
    // moves in fixed-size steps instead of drifting continuously as the
    // camera moves; otherwise each texel covers a different world-space
    // patch every frame and shadow edges shimmer.
    double texelSizeX = (extent * 2.) / double(dLightShadowBuffer.width);
    double texelSizeY = (extent * 2.) / double(dLightShadowBuffer.height);

    Vec3 camPos = cam.GetPos();
    camPos = Vec3(0., 0., 0.);
    float u = camPos.dot(right);
    float w = camPos.dot(camUp);
    float depth = camPos.dot(direction);

    u = float(std::floor(u / texelSizeX) * texelSizeX);
    w = float(std::floor(w / texelSizeY) * texelSizeY);

    Vec3 target = right * u + camUp * w + direction * depth;
    Vec3 eye = target - lightDir * shadowDistance;

    Mat4D p = cam.CreateOrthographicMatrix(near, far, extent, -extent, extent, -extent);
    Mat4D v = lookAt(eye, target, up);

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
        api->width = frameCallbackWidth;
        api->height = frameCallbackHeight;

        // Build perspective matrices
        cam.BuildPerspectiveMatrices(width, height);

        // TODO: rebuild gBuffer

        frameCallbackFlag = false;
    }

    if (mouseCallbackFlag) {
        // The cursor callback reports absolute positions; convert to a
        // frame-to-frame delta relative to the last position we stored.
        float dx = (float)(mouseCallbackX - mouse.pos.x);
        float dy = (float)(mouse.pos.y - mouseCallbackY); // screen-y grows downward; invert

        mouseChange = Vec2(dx, dy);

        mouse.pos.x = mouseCallbackX;
        mouse.pos.y = mouseCallbackY;

        mouseCallbackFlag = false;
        wasMouseMoved = true;
    }
}

void Window::captureMouse() {
    api->captureMouse(true);
}

void Window::uncaptureMouse() {
    api->captureMouse(false);
}

bool Window::isKeyPressed(u32 keycode) {
    return api->isKeyPressed(keycode);
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

