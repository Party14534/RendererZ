#include "main.h"
#include "Objects/Drawable.h"
#include "Shaders/Shaders.h"
#include "Tools/ObjectLoading.h"
#include "global.h"

std::vector<PointVertexAttribute> points = makeTestPointCloud();

Drawable bottom = Drawable::Plane();
Drawable top  = Drawable::Plane();
Drawable front = Drawable::Plane();
Drawable back = Drawable::Plane();
Drawable left = Drawable::Plane();
Drawable right = Drawable::Plane();
Drawable whiteCube = Drawable::Cube();
Drawable redCube = Drawable::Cube();
Drawable blueCube = Drawable::Cube();
Drawable greenCube = Drawable::Cube();
Drawable r7 = Drawable::Cube();
Drawable bunny = Drawable::Object("../src/res/objects/bunny.obj", true);
Drawable teapot = Drawable::Object("../src/res/objects/teapot.obj", true);
Drawable armadillo = Drawable::Object("../src/res/objects/armadillo.obj", true);
Drawable homer = Drawable::Object("../src/res/objects/homer.obj", true);
Drawable cow = Drawable::Object("../src/res/objects/cow.obj", true);
Drawable tung = Drawable::Object("../src/res/objects/tung.obj", false);

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

    Scene s1 = testSceneOne();
    std::shared_ptr<ComplexDrawable> car = LoadComplexDrawableFromFilePath("../src/res/objects/car/scene.gltf");

    std::shared_ptr<PointMesh> pMesh = PointMesh::fromPoints(points);
    pMesh->drawType = GL_POINTS;
    Drawable cloud = Drawable(pMesh);
    cloud.setShader(win.pointShader);
    s1.addToList(&cloud);

    setUpLighting(win);

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
            std::cout << "FPS: " << frameCount << "\n";
            frameCount = 0;
            fpsTimer = t;
        }
        whiteCube.rotateY(t);
        whiteCube.rotateX(t * 0.5f);
        redCube.rotateY(t);
        redCube.rotateX(t * 0.5f);
        blueCube.rotateY(t);
        blueCube.rotateX(t * 0.5f);
        r7.rotateY(-t * .25f);

        bunny.rotateY(t * 2);
        Vec3 p = bunny.getPos();
        bunny.setPos(Vec3(p.x, p.y + .05f * cos(3 * t), p.z));

        float theta = t;
        float r = 4;
        p = tung.getPos() + Vec3(0, 5, 0);
        p.x += r*cos(theta);
        p.z += r*sin(theta);
        win.pLights[0]->setPos(p);
        whiteCube.setPos(p);
        
        p = tung.getPos();
        Vec3 p2 = win.cam.GetPos();
        tung.rotateY(std::atan2(p2.x - p.x, p2.z - p.z));
        
        // Handle rendering
        win.clear(Color(0.f));
        
        //win.draw(s1);
        win.draw(*car);

        win.display();
    }

    return 0;
}

Scene testSceneOne() {
    std::shared_ptr<Texture> tex = Texture::fromFile("../src/res/textures/zari.jpg");
    std::shared_ptr<Texture> tungTex = Texture::fromFile("../src/res/textures/tung.png");

    redCube.setTexture(tex);
    blueCube.setTexture(tex);
    greenCube.setTexture(tex);
    tung.setTexture(tungTex);

    bottom.rotateX(radians(90));
    bottom.setPos(Vec3(0, -20, 0));
    bottom.setScale(Vec3(1000));
    bottom.setColor(Color(1.f));
    top.rotateX(radians(-90));
    top.setPos(Vec3(0, 60, 0));
    top.setScale(Vec3(1000));
    top.setColor(Color(0.1f));
    front.setPos(Vec3(0, 0, 50));
    front.setScale(Vec3(1000));
    front.setColor(Color(0.1f));
    back.rotateY(radians(180));
    back.setPos(Vec3(0, 0, -50));
    back.setScale(Vec3(1000));
    back.setColor(Color(0.1f));

    r7.setColor(Color(.5f, .5f, .5f, 1.f));
    bunny.setColor(Color(.87f, .85f, 1.f, 1.f));
    teapot.setColor(Color(1.f - .87f, 1.f - .85f, 1.f - 1.f, 1.f));
    armadillo.setColor(Color(1.f));
    homer.setColor(Color(float(248)/255,float(219)/255,(float)39/255, 1.));
    cow.setColor(Color(1.f));
    whiteCube.setColor(Color(1000.f));

    bunny.setPos(Vec3(0, 10, 10));
    teapot.setPos(Vec3(0, -10, -10));
    armadillo.setPos(Vec3(0, -10, -15));
    homer.setPos(Vec3(0, -10, 15));
    cow.setPos(Vec3(0, 10, 20));
    r7.setPos(Vec3(9, 5, 9));
    tung.setPos({0, 0, -10});

    whiteCube.setScale(Vec3(.5f));
    redCube.setScale(Vec3(.5f));
    blueCube.setScale(Vec3(.5f));
    greenCube.setScale(Vec3(.5f));
    bunny.setScale(Vec3(50));
    armadillo.setScale(Vec3(.01f));
    r7.setScale(Vec3(2.f));
    tung.setScale(Vec3(.3f));

    armadillo.setMaterial(Material {
        Color(1.f),
        0.f,
        0.f,
        1.f,
        0.f,
        1.f
    });

    homer.setMaterial(armadillo.getMaterial());

    bunny.setMaterial(Material {
            Color(1.f),
            .0f,
            .0f,
            .0f,
            32.f,
            1.f
    });
    cow.setMaterial(bunny.getMaterial());

    r7.setMaterial(Material {
        Color(1.f),
        .2f,
        .4f,
        .0f,
        32.f,
        0.05f
    });

    return Scene({ &bottom, &whiteCube, &redCube,
                &blueCube, &greenCube, &r7, &bunny, &teapot, &armadillo,
                &homer, &cow, &tung });
}

void setUpLighting(Window &win) {
    PointLight l(Vec3(5, 5, 5), PointLightProperties());
    l.setColor(Color(1.f));
    l.setPos(tung.getPos() + Vec3(0, 4, 0));
    l.properties.attenuation = Vec3(1.0, 0.35, .44);
    win.addPointLight(l);

    l.properties.attenuation = Vec3(1.0, 0.045, 0.0075);
    l.setColor(Color(1, 0, 0, 1));
    l.setPos(teapot.getPos() + Vec3(10, 0, 0));
    redCube.setPos(l.getPos() + Vec3(0, -1, 0));
    win.addPointLight(l);

    l.setColor(Color(0., 0., 1., 1.));
    l.setPos(homer.getPos() + Vec3(0, 0, 15));
    blueCube.setPos(l.getPos() + Vec3(0, -1, 0));
    win.addPointLight(l);

    l.setColor(Color(0., 1., 0., 1.));
    l.setPos(homer.getPos() + Vec3(0, 60, 0));
    greenCube.setPos(l.getPos() + Vec3(0, 1, 0));
    win.addPointLight(l);

    win.dLight.setColor(Color(.99f, .56f, .38f, 1));
    win.dLight.properties = DirLightProperties {
        .1, .7, .9,
    };

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
