#include "main.h"

Cube r;
Cube r2;
Cube r3;
Cube r4;
Cube r5;
Cube r6;
Object bunny("../src/res/objects/bunny.obj");
Object teapot("../src/res/objects/teapot.obj");

int main() {
    Window win(800, 600, "Test");
    win.captureMouse();

    Shader customShader("../src/Shaders/vertex.vert", "../src/Shaders/multiTex.frag");
    customShader.use();
    customShader.setInt("tex1", 0);
    customShader.setInt("tex2", 1);
    r.setShader(customShader);

    Texture tex("../src/res/textures/zari.jpg");
    Texture tex2("../src/res/textures/cat.jpg");

    r.setTexture(tex);
    r.addTexture(tex2);

    r2.setTexture(tex);
    r3.setTexture(tex);
    r4.setTexture(tex);
    r5.setTexture(tex);
    r6.setTexture(tex);

    r2.setColor(Color(1.f));
    r3.setColor(Color(1.f, 0.f, 0.f, 1.f));
    r4.setColor(Color(0.f, 1.f, 1.f, 1.f));
    r5.setColor(Color(1.f, 1.f, 0.f, 1.f));
    r6.setColor(Color(1.f, 1.f, 1.f, 1.f));
    bunny.setColor(Color(.87f, .85f, 1.f, 1.f));
    teapot.setColor(Color(1.f - .87f, 1.f - .85f, 1.f - 1.f, 1.f));

    /*Mat m = generateRandomMatrix(4096, 4096);
    Mat m2 = generateRandomMatrix(4096, 4096);

    Mat o = m * m2;*/

    r.setPos(Vec3(0., 0., -3.));
    r.setScale(Vec3(2.f, 2.f, 2.f));

    r2.setPos(Vec3(5.5, 3.5, -5.));
    r3.setPos(Vec3(-5.5, -3.5, -5.));
    r4.setPos(Vec3(0, 0, -60.));
    r5.setPos(Vec3(0, 0, 70.));
    r6.setPos(Vec3(9000, -2000, 0.));
    bunny.setPos(Vec3(0, 10, 10));
    teapot.setPos(Vec3(0, -10, -10));

    r2.setScale(Vec3(1.f, 1.f, 1.f));
    r3.setScale(Vec3(2.f, 2.f, 2.f));
    r4.setScale(Vec3(30.f, 30.f, 30.f));
    r5.setScale(Vec3(50.f, 50.f, 50.f));
    r6.setScale(Vec3(30000.f, 30000.f, 30000.f));
    bunny.setScale(Vec3(50, 50, 50));

    while(win.isOpen())
    {
        // Poll events
        win.pollEvents();
        processInput(win);        

        double dt = glfwGetTime();
        r.setColor(Color(std::sin(dt), std::cos(dt),
                    1. - std::cos(dt), 1.f));
        r.rotateY(dt);
        r.rotateX(dt * 0.5f);
        r2.rotateY(dt);
        r2.rotateX(dt * 0.5f);
        r3.rotateY(dt);
        r3.rotateX(dt * 0.5f);
        r4.rotateY(dt);
        r4.rotateX(dt * 0.5f);
        r5.rotateY(dt);
        r5.rotateX(dt * 0.5f);
        r6.rotateY(-dt);
        r6.rotateX(-dt * 0.5f);
        r6.rotateZ(-dt * 0.5f);
        
        // Handle rendering
        win.clear(Color(1.f));

        customShader.use();
        customShader.setFloat("dt", dt);
        customShader.setColor("color", r.getColor());

        win.draw(r);

        win.draw(r2);
        win.draw(r3);
        win.draw(r4);
        win.draw(r5);
        win.draw(bunny);
        win.draw(teapot);

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
