#include "main.h"
#include <GLFW/glfw3.h>
#include <cmath>

Tri t;
Rect r;

int main() {
    Window win(800, 600, "Test");

    Texture tex("../src/res/textures/zari.jpg");
    t.setTexture(tex);

    while(win.isOpen())
    {
        glfwPollEvents();    
        r.setColor(Color(.67f, .69f, std::sin(glfwGetTime()), 1.f));

        // Handle rendering
        win.clear(Color(.88f, .76f, 1.f, 1.f));

        win.draw(r);
        win.draw(t);

        win.display();
    }

    return 0;
}
