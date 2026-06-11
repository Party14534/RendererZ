#include "main.h"
#include "global.h"
#include <Objects/LightSource.h>

Cube r2;
Cube r3;
Cube r7;
Object bunny("../src/res/objects/bunny.obj", true);
Object teapot("../src/res/objects/teapot.obj", true);
Object armadillo("../src/res/objects/armadillo.obj", true);
Object homer("../src/res/objects/homer.obj", true);
Object cow("../src/res/objects/cow.obj", true);

int main() {
    Window win(800, 600, "Test");
    win.captureMouse();

    LightSource l(Cube::_defaultVerts, Cube::_defaultIndices);

    Texture tex("../src/res/textures/zari.jpg");
    Texture tex2("../src/res/textures/cat.jpg");


    r2.setTexture(tex);
    r3.setTexture(tex);

    r2.setColor(Color(1.f));
    r3.setColor(Color(1.f, 0.f, 0.f, 1.f));
    r7.setColor(Color(.5f, .5f, .5f, 1.f));
    bunny.setColor(Color(.87f, .85f, 1.f, 1.f));
    teapot.setColor(Color(1.f - .87f, 1.f - .85f, 1.f - 1.f, 1.f));
    armadillo.setColor(Color(1.f));
    homer.setColor(Color(float(248)/255,float(219)/255,(float)39/255, 1.));
    cow.setColor(Color(1.f));
    l.setColor(Color(.88, .85, 1., 1.));

    /*Mat m = generateRandomMatrix(4096, 4096);
    Mat m2 = generateRandomMatrix(4096, 4096);

    Mat o = m * m2;*/


    r2.setPos(Vec3(5.5, 3.5, -5.));
    r3.setPos(Vec3(-5.5, -3.5, -5.));
    bunny.setPos(Vec3(0, 10, 10));
    teapot.setPos(Vec3(0, -10, -10));
    armadillo.setPos(Vec3(0, -10, -15));
    homer.setPos(Vec3(0, -10, 15));
    cow.setPos(Vec3(0, 10, 20));
    l.setPos(Vec3(5, 5, 5));
    r7.setPos(Vec3(9, 5, 9));

    r2.setScale(Vec3(1.f));
    r3.setScale(Vec3(2.f));
    bunny.setScale(Vec3(50));
    armadillo.setScale(Vec3(.01f));
    r7.setScale(Vec3(2.f));

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

    while(win.isOpen())
    {
        lightPos = l.getPos();
        lightCol = l.getColor();
        lightProp = l.properties;

        // Poll events
        win.pollEvents();
        processInput(win);        

        double dt = glfwGetTime();
        r2.rotateY(dt);
        r2.rotateX(dt * 0.5f);
        r3.rotateY(dt);
        r3.rotateX(dt * 0.5f);
        r7.rotateY(-dt * .25f);

        bunny.rotateY(dt * 2);
        Vec3 p = bunny.getPos();
        bunny.setPos(Vec3(p.x, p.y + .05f * cos(3 * dt), p.z));
        
        // Handle rendering
        win.clear(Color(0.f));

        win.draw(r2);
        win.draw(r3);
        win.draw(bunny);
        win.draw(teapot);
        win.draw(armadillo);
        win.draw(homer);
        win.draw(cow);
        win.draw(l);
        win.draw(r7);

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
