#include "main.h"

Rect r;
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
    Texture tex2("../src/res/textures/cat.jpg");

    r.setTexture(tex);
    r.addTexture(tex2);
    
    t.setTexture(tex);
    t.addTexture(tex2);

    Mat translation = Mat(4,4,{
            1,0,0,1,
            0,1,0,2,
            0,0,1,3,
            0,0,0,1
            });
    Mat scale = Mat(4,4,{
            2,0,0,0,
            0,2,0,0,
            0,0,2,0,
            0,0,0,1
            });

    Mat ts = translation * scale;
    std::cout << ts;

    Vec4 pos = ts * Vec4(1.f,1.f,1.f,1.f);

    Mat m = generateRandomMatrix(4096, 4096);
    Mat m2 = generateRandomMatrix(4096, 4096);

    Mat o = m * m2;

    std::cout << pos.x << "," <<pos.y <<","<<pos.z<<","<<pos.w<<"\n";

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
