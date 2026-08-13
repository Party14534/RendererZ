#include "TestSceneOne.h"
#include "Objects/Drawable.h"
#include "Objects/Texture.h"

namespace {
    Drawable bottom = Drawable::Plane();
    Drawable top  = Drawable::Plane();
    Drawable front = Drawable::Plane();
    Drawable back = Drawable::Plane();
    Drawable left = Drawable::Plane();
    Drawable right = Drawable::Plane();
    Drawable whiteCube = Drawable::Cube();
    Drawable redCube = Drawable::Cube();
    Drawable blueCube = Drawable::Cube();
    Drawable greenCube = Drawable::Cube();
    Drawable r7 = Drawable::Cube();
    Drawable bunny = Drawable::Object("../src/res/objects/bunny.obj", true);
    Drawable teapot = Drawable::Object("../src/res/objects/teapot.obj", true);
    Drawable armadillo = Drawable::Object("../src/res/objects/armadillo.obj", true);
    Drawable homer = Drawable::Object("../src/res/objects/homer.obj", true);
    Drawable cow = Drawable::Object("../src/res/objects/cow.obj", true);
    Drawable tung = Drawable::Object("../src/res/objects/tung.obj", false);
}

Scene testSceneOne() {
    std::shared_ptr<Texture> zariTex = Texture::fromFile("../src/res/textures/zari.jpg");
    std::shared_ptr<Texture> tungTex = Texture::fromFile("../src/res/textures/tung.png");
    std::shared_ptr<Texture> wallTex = Texture::fromFile("../src/res/textures/wall/wall_diffuse.png");
    std::shared_ptr<Texture> wallNormalTex = Texture::fromFile("../src/res/textures/wall/wall_normal.png");

    redCube.setDiffuseTexture(zariTex);
    blueCube.setDiffuseTexture(zariTex);
    greenCube.setDiffuseTexture(zariTex);
    tung.setDiffuseTexture(tungTex);
    bottom.setDiffuseTexture(wallTex);
    bottom.setNormalTexture(wallNormalTex);
    r7.setDiffuseTexture(wallTex);
    r7.setNormalTexture(wallNormalTex);

    bottom.rotateX(radians(90));
    bottom.setPos(Vec3(0, -20, 0));
    bottom.setScale(Vec3(1000));
    bottom.setColor(Color(1.f));
    bottom.setUVScale(Vec2(100, 100));
    top.rotateX(radians(-90));
    top.setPos(Vec3(0, 60, 0));
    top.setScale(Vec3(1000));
    top.setColor(Color(0.1f));
    front.setPos(Vec3(0, 0, 50));
    front.setScale(Vec3(1000));
    front.setColor(Color(0.1f));
    back.rotateY(radians(180));
    back.setPos(Vec3(0, 0, -50));
    back.setScale(Vec3(1000));
    back.setColor(Color(0.1f));

    r7.setColor(Color(1.f, 1.f, 1.f, 1.f));
    bunny.setColor(Color(.87f, .85f, 1.f, 1.f));
    teapot.setColor(Color(1.f - .87f, 1.f - .85f, 1.f - 1.f, 1.f));
    armadillo.setColor(Color(1.f));
    homer.setColor(Color(float(248)/255,float(219)/255,(float)39/255, 1.));
    cow.setColor(Color(1.f));
    whiteCube.setColor(Color(1000.f));

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
        0.f,
        1.f
    });

    homer.setMaterial(armadillo.getMaterial());

    bunny.setMaterial(Material {
            Color(1.f),
            .0f,
            .0f,
            .0f,
            32.f,
            1.f
    });
    cow.setMaterial(bunny.getMaterial());

    return Scene({ &bottom, &whiteCube, &redCube,
                &blueCube, &greenCube, &r7, &bunny, &teapot, &armadillo,
                &homer, &cow, &tung });
}

void setUpLighting(Window &win) {
    PointLight l(Vec3(5, 5, 5), PointLightProperties());
    l.setColor(Color(1.f));
    l.setPos(tung.getPos() + Vec3(0, 4, 0));
    l.properties.attenuation = Vec3(1.0, 0.045, .0075);
    win.addPointLight(l);

    l.properties.attenuation = Vec3(1.0, 0.045, 0.0075);
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

    win.dLight.setColor(Color(.99f, .56f, .38f, 1));
    win.dLight.properties = DirLightProperties {
        .2, .7, .9,
    };
}

void updateTestSceneOne(double t, Window& win) {
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
    win.pLights[0]->setPos(p);
    whiteCube.setPos(p);

    p = tung.getPos();
    Vec3 p2 = win.cam.GetPos();
    tung.rotateY(std::atan2(p2.x - p.x, p2.z - p.z));
}
