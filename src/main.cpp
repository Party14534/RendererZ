#include "main.h"
#include <GLFW/glfw3.h>
#include <cmath>

Tri t(std::vector<Vec3> {
        Vec3(-0.5f, -0.5f, 0.0f),
        Vec3(0.5f, -0.5f, 0.0f),
        Vec3(0.0f,  0.5f, 0.0f)
});

Tri t2(std::vector<Vec3> {
        Vec3(-1.f, -1.f, 0.0f),
        Vec3(1.f, -1.f, 0.0f),
        Vec3(-1.0f,  1.f, 0.0f)
});

Rect r1(std::vector<Vec3> {
            Vec3(.5f, .5f, 0.0f),
            Vec3(.5f, -.5f, 0.0f),
            Vec3(-.5f, -.5f, 0.0f),
            Vec3(-.5f, .5f, 0.0f)
        },
        std::vector<u32> {
            0,1,3,
            1,2,3
});

int main() {
    Window win(800, 600, "Test");

    while(win.isOpen())
    {
        glfwPollEvents();    
        r1.setColor(Color(.67f, .69f, std::sin(glfwGetTime()), 1.f));
        t.setColor(Color(.67f, .69f, std::cos(glfwGetTime()), 1.f));

        // Handle rendering
        win.clear(Color(.88f, .76f, 1.f, 1.f));

        //win.draw(t2);
        win.draw(r1);
        win.draw(t);
    }

    return 0;
}
