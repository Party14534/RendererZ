#include "main.h"
#include <GLFW/glfw3.h>
#include <cmath>

Rect r;
Tri t;

int main() {
    Window win(800, 600, "Test");

    std::cout << "initializing custom\n";
    Shader customShader("../src/Shaders/vertex.vert", "../src/Shaders/multiTex.frag");
    customShader.use();
    customShader.setInt("tex1", 0);
    customShader.setInt("tex2", 1);
    r.setShader(customShader);
    t.setShader(customShader);

    Texture tex("../src/res/textures/zari.jpg");
    Texture tex2("../src/res/textures/cat.jpg");

    r.setTexture(tex);
    r.addTexture(tex2);
    
    t.setTexture(tex);
    t.addTexture(tex2);

    /*Mat4 x({4.f, 2.f, 0.f, 0.f, 0.f, 8.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f});
    Mat4 y({4.f, 2.f, 1.f, 0.f, 2.f, 0.f, 4.f, 0.f, 9.f, 4.f, 2.f, 0.f});
    Mat4 o = x * y;
    std::cout << o;*/

    while(win.isOpen())
    {
        glfwPollEvents();    
        double dt = glfwGetTime();
        r.setColor(Color(std::sin(dt), std::cos(dt),
                    1. - std::cos(dt), 1.f));
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
