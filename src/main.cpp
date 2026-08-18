#include "main.h"
#include "Objects/Drawable.h"
#include "Objects/Texture.h"
#include "Shaders/shaders.h"
#include "Tools/ObjectLoading.h"
#include "Scenes/TestSceneOne.h"
#include "Scenes/ManyLightsScene.h"
#include "Scenes/SponzaScene.h"
#include "global.h"

enum class SceneMode { TestScene, ManyLights, Sponza };

std::vector<PointVertexAttribute> points = makeTestPointCloud();

int main() {
    Window win(800, 600, "Test");
    win.captureMouse();

    SkyBox sBox({
            "../src/res/textures/squareKitty.jpg",
            "../src/res/textures/squareKitty.jpg",
            "../src/res/textures/squareKitty.jpg",
            "../src/res/textures/squareKitty.jpg",
            "../src/res/textures/squareKitty.jpg",
            "../src/res/textures/squareKitty.jpg"
    });
    win.setSkyBox(std::make_shared<SkyBox>(sBox));

    Scene sceneOne = testSceneOne();
    Scene lightScene = testManyLightsScene();
    std::shared_ptr<ComplexDrawable> sponza = loadSponza();
    std::shared_ptr<ComplexDrawable> car = LoadComplexDrawableFromFilePath("../src/res/objects/car/scene.gltf");
    std::shared_ptr<ComplexDrawable> phoenix = LoadComplexDrawableFromFilePath("../src/res/objects/phoenix_bird/scene.gltf");

    /* TODO: Set up points again
    std::shared_ptr<PointMesh> pMesh = PointMesh::fromPoints(points);
    pMesh->drawType = GL_POINTS;
    Drawable cloud = Drawable(pMesh);
    cloud.setShader(win.pointShader);
    s1.addToList(&cloud);*/

    setUpLighting(win);

    SceneMode sceneMode = SceneMode::TestScene;
    bool toggleKeyWasPressed = false;
    bool saoToggleKeyWasPressed = false;

    int frameCount = 0;
    double fpsTimer = glfwGetTime();

    while(win.isOpen())
    {
        // Poll events
        win.pollEvents();
        processInput(win);

        //applyGravity(pMesh->points);
        //pMesh->updateBuffer();

        double t = glfwGetTime();

        frameCount++;
        if (t - fpsTimer >= 1.0) {
            std::cout << "FPS: " << frameCount << " (" << win.pLights.size() << " point lights)\n";
            frameCount = 0;
            fpsTimer = t;
        }

        if (win.isKeyPressed(GLFW_KEY_M) && !toggleKeyWasPressed) {
            switch (sceneMode) {
                case SceneMode::TestScene:  sceneMode = SceneMode::ManyLights; break;
                case SceneMode::ManyLights: sceneMode = SceneMode::Sponza;     break;
                case SceneMode::Sponza:     sceneMode = SceneMode::TestScene;  break;
            }

            win.pLights.clear();
            switch (sceneMode) {
                case SceneMode::TestScene:  setUpLighting(win);         break;
                case SceneMode::ManyLights: setUpManyPointLights(win);  break;
                case SceneMode::Sponza:     setUpSponzaLighting(win);   break;
            }
        }
        toggleKeyWasPressed = win.isKeyPressed(GLFW_KEY_M);

        if (win.isKeyPressed(GLFW_KEY_N) && !saoToggleKeyWasPressed) {
            win.showSao = !win.showSao;
        }
        saoToggleKeyWasPressed = win.isKeyPressed(GLFW_KEY_N);

        switch (sceneMode) {
            case SceneMode::TestScene:  updateTestSceneOne(t, win);   break;
            case SceneMode::ManyLights: updateManyLightsScene(t, win); break;
            case SceneMode::Sponza:     updateSponzaScene(t, win);     break;
        }

        // Handle rendering
        win.clear(Color(0.f));

        switch (sceneMode) {
            case SceneMode::TestScene:
                win.draw(sceneOne);
                win.draw(*phoenix);
                win.draw(*car);
                break;
            case SceneMode::ManyLights:
                win.draw(lightScene);
                break;
            case SceneMode::Sponza:
                win.draw(*sponza);
                break;
        }

        win.display();
    }

    return 0;
}

void processInput(Window& win) {
    static bool escapePressed = false;
    float camSpeed = 0.05f;
    if (win.isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
        camSpeed *= 2.f;
    }

    if (win.isKeyPressed(GLFW_KEY_ESCAPE)) {
        win.uncaptureMouse();
        escapePressed = true;
    } else if (escapePressed) {
        win.captureMouse();
        escapePressed = false;
    }

    if(win.isKeyPressed(GLFW_KEY_W)) {
        win.cam.MovePos(win.cam.GetCamFront() * camSpeed);
    } else if(win.isKeyPressed(GLFW_KEY_S)) {
        win.cam.MovePos(win.cam.GetCamFront() * -camSpeed);
    } 

    if(win.isKeyPressed(GLFW_KEY_A)) {
        win.cam.MovePos(win.cam.GetCamFront().cross(win.cam.GetCamUp()) * -camSpeed);
    } else if(win.isKeyPressed(GLFW_KEY_D)) {
        win.cam.MovePos(win.cam.GetCamFront().cross(win.cam.GetCamUp()) * camSpeed);
    }

    if (win.isKeyPressed(GLFW_KEY_SPACE)) {
        win.cam.MovePos(win.cam.GetCamUp() * camSpeed);
    } else if (win.isKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
        win.cam.MovePos(win.cam.GetCamUp() * -camSpeed);
    }

    if (win.wasMouseMoved) {
        win.mouseChange.y *= -1;
        win.cam.MoveDirection(win.mouseChange * 0.1f);
    }
}

std::vector<PointVertexAttribute> makeTestPointCloud() {
    std::vector<PointVertexAttribute> pts;
    const int size = 10;
    const float sizeF = size;
    const float spacing = 1.5f;
    srand(time(NULL));

    pts.reserve(size * size);

    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            for (int z = 0; z < size; z++) {
                float offsetX = rand() % (size >> 1);
                float offsetY = rand() % (size >> 1);
                float offsetZ = rand() % (size >> 1);
                float px = (float)(x - sizeF / 2) * spacing + offsetX;
                float py = (float)(y - sizeF / 2) * spacing - offsetY;
                float pz = (float)(z - sizeF / 2) * spacing - 6.f + offsetZ; // sit in front of the starting camera

                float r = (float)x / (size - 1);
                float g = (float)y / (size - 1);
                float b = (float)z / (size - 1);

                pts.push_back(PointVertexAttribute(px, py, pz, r, g, b));
            }
        }
    }

    return pts;
}

void applyGravity(std::vector<PointVertexAttribute>& points) {
    const float G = .1f;          // scaled way up for visibility - real G (6.674e-11) is imperceptible at this scale
    const float softening = 0.75f; // keeps 1/r^2 bounded when points coincide or share an axis (a grid has plenty of both)
    std::vector<PointVertexAttribute> snapshot = points; // stable positions to read while we write to the live array

    std::for_each(
        std::execution::par,
        points.begin(),
        points.end(),
        [&points, &snapshot, G, softening](PointVertexAttribute& p) {
            u32 selfIndex = (u32)(&p - points.data());
            Vec3 v;
            Vec3 pos(p.x, p.y, p.z);
            for (u32 i = 0; i < snapshot.size(); i++) {
                if (i == selfIndex) continue;
                const PointVertexAttribute& p2 = snapshot[i];
                Vec3 pos2(p2.x, p2.y, p2.z);

                Vec3 d = pos2 - pos;
                float r2 = d.dot(d) + softening * softening;
                float r = sqrt(r2);
                Vec3 dir = d * (1.f / r);  // unit vector pointing toward the other point
                float accel = G / r2;      // inverse-square law, assuming unit mass

                v = v + dir * accel;
            }

            pos = pos + v * 0.00167f;
            p.x = pos.x;
            p.y = pos.y;
            p.z = pos.z;
        }
    );
}
