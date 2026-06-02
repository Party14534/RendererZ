#include "main.h"

Cube r;
Cube r2;
Cube r3;
Cube r4;

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

    r2.setColor(Color(1.f));
    r3.setColor(Color(1.f, 0.f, 0.f, 1.f));
    r4.setColor(Color(0.0f, 1.f, 1.f, 1.f));

    /*Mat m = generateRandomMatrix(4096, 4096);
    Mat m2 = generateRandomMatrix(4096, 4096);

    Mat o = m * m2;*/

    r.setPos(Vec3(0., 0., -3.));
    r.setScale(Vec3(2.f, 2.f, 2.f));

    r2.setPos(Vec3(5.5, 3.5, -5.));
    r3.setPos(Vec3(-5.5, -3.5, -5.));
    r4.setPos(Vec3(0, 0, -60.));

    r2.setScale(Vec3(1.f, 1.f, 1.f));
    r3.setScale(Vec3(2.f, 2.f, 2.f));
    r4.setScale(Vec3(30.f, 30.f, 30.f));

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
        
        // Handle rendering
        win.clear(Color());

        customShader.use();
        customShader.setFloat("dt", dt);
        customShader.setColor("color", r.getColor());

        win.draw(r);

        win.draw(r2);
        win.draw(r3);
        win.draw(r4);

        win.display();
    }

    return 0;
}

void processInput(Window& win) {
    float camSpeed = 0.05f;
    if (win.isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
        camSpeed *= 2.f;
        if (win.isKeyPressed(GLFW_KEY_ESCAPE)) {
            win.captureMouse();
        }
    } else if (win.isKeyPressed(GLFW_KEY_ESCAPE)) {
        win.uncaptureMouse();
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
