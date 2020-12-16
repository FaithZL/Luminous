//
// Created by Zero on 2020/10/19.
//

#include <compute/dsl_syntax.h>
#include "render/include/light.h"

namespace luminous::render::light {
    using luminous::render::AreaLight;
    class DiffuseAreaLight : public AreaLight {
    private:
        const float3 _emission;
    public:
        DiffuseAreaLight(Device *device, const ParamSet &params) noexcept
        : AreaLight{device, params} {

        }
    };
}