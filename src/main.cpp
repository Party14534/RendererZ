#include "main.h"

Cube r;
Tri t;

int main() {
    Window win(800, 600, "Test");

    Shader customShader("../src/Shaders/vertex.vert", "../src/Shaders/multiTex.frag");
    customShader.use();
    customShader.setInt("tex1", 0);
    customShader.setInt("tex2", 1);
    r.setShader(customShader);
    t.setShader(customShader);

    Texture tex("../src/res/textures/zari.jpg");
    Texture tex2("../src/res/textures/zari.jpg");

    r.setTexture(tex);
    r.addTexture(tex2);
    
    t.setTexture(tex);
    t.addTexture(tex2);

    /*Mat m = generateRandomMatrix(4096, 4096);
    Mat m2 = generateRandomMatrix(4096, 4096);

    Mat o = m * m2;*/

    r.setPos(Vec3(0., 0., -3.));
    r.setScale(Vec3(2.f, 2.f, 1.f));

    t.setPos(Vec3(0.1, 0.3, 0));
    t.setScale(Vec3(0.25f, 0.25f, 0.25f));

    while(win.isOpen())
    {
        // Poll events
        glfwPollEvents();    

        double dt = glfwGetTime();
        r.setColor(Color(std::sin(dt), std::cos(dt),
                    1. - std::cos(dt), 1.f));
        r.rotateZ(dt);
        t.rotateZ(-dt);

        t.setColor(Color(std::cos(dt), std::sin(dt),
                    1. - std::sin(dt), 1.f));
        
        // Handle rendering
        win.clear(Color(.88f, .76f, 1.f, 1.f));

        customShader.use();
        customShader.setFloat("dt", dt);
        customShader.setColor("color", r.getColor());

        win.draw(r);

        customShader.use();
        customShader.setFloat("dt", 1. - dt);
        customShader.setColor("color", t.getColor());

        //win.draw(t);

        win.display();
    }

    return 0;
}
