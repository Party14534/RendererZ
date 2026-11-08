#include "Material.h"

shared<PBRMaterial> CreateDefaultMaterial() {
    return std::make_shared<PBRMaterial>(nullptr, nullptr, nullptr,
            Color(1.), 1., 1.);
}
