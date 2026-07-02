#include "main.h"
#include "Math/math.h"

Cube whiteCube;
Cube redCube;
Cube blueCube;
Cube greenCube;
Cube r7;
Object bunny("../src/res/objects/bunny.obj", true);
Object teapot("../src/res/objects/teapot.obj", true);
Object armadillo("../src/res/objects/armadillo.obj", true);
Object homer("../src/res/objects/homer.obj", true);
Object cow("../src/res/objects/cow.obj", true);
Object tung("../src/res/objects/tung.obj", false);

int main() {
    Window win(800, 600, "Test");
    win.captureMouse();

    PointLight l(Vec3(5, 5, 5), PointLightProperties());
    
    Texture tex("../src/res/textures/zari.jpg");
    Texture tex2("../src/res/textures/cat.jpg");
    Texture tungTex("../src/res/textures/tung.png");

    redCube.setTexture(tex);
    blueCube.setTexture(tex);
    greenCube.setTexture(tex);
    tung.setTexture(tungTex);

    r7.setColor(Color(.5f, .5f, .5f, 1.f));
    bunny.setColor(Color(.87f, .85f, 1.f, 1.f));
    teapot.setColor(Color(1.f - .87f, 1.f - .85f, 1.f - 1.f, 1.f));
    armadillo.setColor(Color(1.f));
    homer.setColor(Color(float(248)/255,float(219)/255,(float)39/255, 1.));
    cow.setColor(Color(1.f));
    whiteCube.setColor(Color(1000.f));

    /*Mat m = generateRandomMatrix(4096, 4096);
    Mat m2 = generateRandomMatrix(4096, 4096);

    Mat o = m * m2;*/


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
        0.f
    });

    homer.setMaterial(armadillo.getMaterial());

    bunny.setMaterial(Material {
            Color(1.f),
            .2f,
            .4f,
            .0f,
            32.f
    });
    cow.setMaterial(bunny.getMaterial());

    l.setColor(Color(1.f));
    l.setPos(tung.getPos() + Vec3(0, 4, 0));
    l.properties.attenuation = Vec3(1.0, 0.056, 0.0028);
    win.addPointLight(l);

    l.properties.attenuation = Vec3(1.0, 0.014, 0.0007);
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

    win.dLight.setColor(Color(1.));
    win.dLight.properties = DirLightProperties {
        .1, .3, .1,
    };


    while(win.isOpen())
    {
        // Poll events
        win.pollEvents();
        processInput(win);        

        double t = glfwGetTime();
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
        win.pLights[0].setPos(p);
        whiteCube.setPos(p);
        
        // Handle rendering
        win.clear(Color(0.f));

        win.draw(whiteCube);
        win.draw(redCube);
        win.draw(blueCube);
        win.draw(greenCube);
        win.draw(bunny);
        win.draw(teapot);
        win.draw(armadillo);
        win.draw(homer);
        win.draw(cow);
        win.draw(r7);

        win.draw(tung);

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
