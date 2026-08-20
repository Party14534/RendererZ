#include "SponzaScene.h"
#include "Tools/ObjectLoading.h"

std::shared_ptr<ComplexDrawable> loadSponza() {
    return LoadComplexDrawableFromFilePath("../src/res/objects/sponza/Sponza.gltf");
}

void setUpSponzaLighting(Window& win) {
    win.dLight.setColor(Color(1.f, .95f, .85f, 1.f));
    win.dLight.setDir(Vec3(-0.5f, -1.f, -0.3f));
    win.dLight.properties = DirLightProperties {
        .2f, .8f, .4f,
    };

    /*PointLightProperties props;
    props.attenuation = Vec3(1.0f, 0.045f, 0.0075f);

    PointLight warm(Vec3(-30, 6, 0), props);
    warm.setColor(Color(1.f, .6f, .3f, 1.f));
    win.addPointLight(warm);

    PointLight cool(Vec3(30, 6, 0), props);
    cool.setColor(Color(.3f, .6f, 1.f, 1.f));
    win.addPointLight(cool);

    PointLight center(Vec3(0, 20, 0), props);
    center.setColor(Color(1.f));
    win.addPointLight(center);*/
}

void updateSponzaScene(double t, Window& win) {
    // Sweep the directional light's azimuth around the vertical axis,
    // completing one full rotation every 30 seconds.
    const Vec3 baseDir(-0.5f, -1.f, -0.3f);
    const double twoPi = 6.283185307179586;
    double angle = t * (twoPi / 30.);
    float cosA = (float)cos(angle);
    float sinA = (float)sin(angle);
    win.dLight.setDir(Vec3(
        baseDir.x * cosA - baseDir.z * sinA,
        baseDir.y,
        baseDir.x * sinA + baseDir.z * cosA
    ));

    if (win.pLights.size() < 3) return;

    win.pLights[0]->setPos(Vec3(-30, 6, (float)sin(t * .3) * 20));
    win.pLights[1]->setPos(Vec3(30, 6, (float)cos(t * .3) * 20));
}
