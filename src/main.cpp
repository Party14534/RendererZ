#include "main.h"

Cube r;
Cube r2;
Cube r3;
Cube r4;

int main() {
    Window win(800, 600, "Test");

    Shader customShader("../src/Shaders/vertex.vert", "../src/Shaders/multiTex.frag");
    customShader.use();
    customShader.setInt("tex1", 0);
    customShader.setInt("tex2", 1);
    r.setShader(customShader);

    Texture tex("../src/res/textures/zari.jpg");
    Texture tex2("../src/res/textures/zari.jpg");
    Texture tex3("../src/res/textures/zari.jpg");

    r.setTexture(tex);
    r.addTexture(tex2);

    r2.setTexture(tex3);
    r2.setColor(Color(1.f));
    r3.setColor(Color(0.84, 0.80, 1.f, 1.f));
    r4.setColor(Color(0.80, 0.92, 1.f, 1.f));

    /*Mat m = generateRandomMatrix(4096, 4096);
    Mat m2 = generateRandomMatrix(4096, 4096);

    Mat o = m * m2;*/

    r.setPos(Vec3(0., 0., -3.));
    r.setScale(Vec3(2.f, 2.f, 2.f));

    r2.setPos(Vec3(5.5, 3.5, -5.));
    r3.setPos(Vec3(-5.5, -3.5, -5.));
    r4.setPos(Vec3(5.5, -3.5, -5.));

    r2.setScale(Vec3(1.f, 1.f, 1.f));
    r3.setScale(Vec3(2.f, 2.f, 2.f));
    r4.setScale(Vec3(3.f, 3.f, 3.f));

    while(win.isOpen())
    {
        // Poll events
        glfwPollEvents();    

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
        win.clear(Color(.88f, .76f, 1.f, 1.f));

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
