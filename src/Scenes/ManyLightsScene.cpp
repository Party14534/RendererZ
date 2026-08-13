#include "ManyLightsScene.h"
#include "Objects/Drawable.h"
#include <cstdlib>
#include <cmath>

namespace {
    // Deferred shading's light pass cost scales with lights x screen pixels,
    // not lights x objects, so a wide flat receiver plus a modest object
    // count isolates the light-count cost the demo is meant to show.
    const int GRID_SIZE = 6;
    const float SPACING = 14.f;
    const int LIGHT_COUNT = 800;
    const float FLOOR_MARGIN = 10.f; // extra floor beyond the light-scatter radius

    struct ModelSpec {
        std::string path;
        bool genNormals;
        float scale;
    };

    // Reuses the same model files/scales as testSceneOne.
    const std::vector<ModelSpec> MODEL_SPECS = {
        { "../src/res/objects/bunny.obj", true, 50.f },
        { "../src/res/objects/teapot.obj", true, 2.f },
        { "../src/res/objects/armadillo.obj", true, .05f },
        { "../src/res/objects/homer.obj", true, 7.f },
        { "../src/res/objects/cow.obj", true, 1.f },
        { "../src/res/objects/tung.obj", false, .5f },
    };

    float randRange(float lo, float hi) {
        return lo + (float)rand() / RAND_MAX * (hi - lo);
    }

    // Independent random RGB channels tend toward pale/gray (their average
    // dominates over their spread); HSV with full saturation/value guarantees
    // a vivid, fully-saturated hue every time.
    Color randVividColor() {
        float h = randRange(0.f, 360.f);
        float c = 1.f; // saturation * value, both 1
        float x = c * (1.f - std::fabs(std::fmod(h / 60.f, 2.f) - 1.f));
        float r, g, b;
        if      (h < 60)  { r = c; g = x; b = 0; }
        else if (h < 120) { r = x; g = c; b = 0; }
        else if (h < 180) { r = 0; g = c; b = x; }
        else if (h < 240) { r = 0; g = x; b = c; }
        else if (h < 300) { r = x; g = 0; b = c; }
        else              { r = c; g = 0; b = x; }
        return Color(r, g, b, 1.f);
    }

    float meshMinY(const Drawable& d) {
        float minY = 0.f;
        bool first = true;
        for (const VertexAttribute& v : d.mesh->vertices) {
            if (first || v.y < minY) minY = v.y;
            first = false;
        }
        return minY;
    }

    std::vector<Drawable> makePrototypes() {
        std::vector<Drawable> prototypes;
        prototypes.reserve(MODEL_SPECS.size());
        for (const ModelSpec& spec : MODEL_SPECS) {
            Drawable d = Drawable::Object(spec.path, spec.genNormals);
            d.setColor(Color(1.f)); // neutral white so each light's own color reads clearly
            d.setScale(Vec3(spec.scale));
            prototypes.push_back(d);
        }
        return prototypes;
    }

    Drawable floorPlane = Drawable::Plane();
    std::vector<Drawable> prototypes = makePrototypes();
    std::vector<Drawable> gridObjects;

    struct LightMotion {
        Vec3 center;
        float radius;
        float speed;
        float phase;
        float vAmplitude;
        float vSpeed;
        float vPhase;
    };

    std::vector<LightMotion> lightMotion;
}

Scene testManyLightsScene() {
    float extent = GRID_SIZE * SPACING;
    float floorY = -1.f;

    floorPlane.rotateX(radians(90));
    floorPlane.setPos(Vec3(0, floorY, 0));
    floorPlane.setScale(Vec3(extent + FLOOR_MARGIN * 2.f));
    floorPlane.setColor(Color(1.f));

    Scene s;
    s.addToList(&floorPlane);

    gridObjects.clear();
    gridObjects.reserve(GRID_SIZE * GRID_SIZE);

    float origin = (GRID_SIZE - 1) * SPACING * 0.5f;
    for (int x = 0; x < GRID_SIZE; x++) {
        for (int z = 0; z < GRID_SIZE; z++) {
            const Drawable& proto = prototypes[(x * GRID_SIZE + z) % prototypes.size()];
            gridObjects.push_back(proto);
            Drawable& obj = gridObjects.back();

            float scale = obj.getScale().x;
            float groundOffset = -meshMinY(obj) * scale; // rest the mesh's lowest vertex on the floor
            obj.setPos(Vec3(x * SPACING - origin, floorY + groundOffset, z * SPACING - origin));
            obj.rotateY(randRange(0.f, 6.283f));

            s.addToList(&obj);
        }
    }

    return s;
}

void setUpManyPointLights(Window& win) {
    win.dLight.properties = DirLightProperties { 0.f, 0.f, 0.f };

    float extent = GRID_SIZE * SPACING * 0.5f;

    lightMotion.clear();
    lightMotion.reserve(LIGHT_COUNT);

    for (int i = 0; i < LIGHT_COUNT; i++) {
        PointLightProperties props;
        // Tight falloff: with 100 lights, a loose radius means many overlap
        // at any given point and their colors average out to white.
        props.attenuation = Vec3(1.0f, 1.2f, 2.5f);

        Vec3 center(randRange(-extent, extent), randRange(2.f, 6.f), randRange(-extent, extent));
        PointLight l(center, props);
        l.setColor(randVividColor());
        win.addPointLight(l);

        lightMotion.push_back(LightMotion {
            center,
            randRange(1.f, 4.f),
            randRange(0.3f, 1.2f),
            randRange(0.f, 6.283f),
            randRange(0.5f, 1.5f),
            randRange(0.5f, 2.f),
            randRange(0.f, 6.283f),
        });
    }
}

void updateManyLightsScene(double t, Window& win) {
    u32 count = win.pLights.size() < lightMotion.size() ? win.pLights.size() : lightMotion.size();

    for (u32 i = 0; i < count; i++) {
        const LightMotion& m = lightMotion[i];
        float angle = (float)t * m.speed + m.phase;
        Vec3 pos = m.center + Vec3(
            cos(angle) * m.radius,
            sin((float)t * m.vSpeed + m.vPhase) * m.vAmplitude,
            sin(angle) * m.radius
        );
        win.pLights[i]->setPos(pos);
    }
}
