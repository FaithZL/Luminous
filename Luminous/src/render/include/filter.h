//
// Created by Zero on 2020/10/17.
//

#pragma once

#include <render/plugin.h>
#include <compute/dsl.h>

namespace luminous::render {

    using compute::Device;

    class Filter : public Plugin {
    private:
        float _radius;
    public:
        Filter(Device *device, const nloJson &params)
        : Plugin(device, params),
        _radius(params.value("radius", 1)) {

        }

        [[nodiscard]] float radius() const noexcept {
            return _radius;
        }

        // Filter 1D weight function, offset is in range [-radius, radius)
        [[nodiscard]] virtual float _weight_1d(float offset) const noexcept = 0;

        // (position, weight)
        [[nodiscard]] virtual std::pair<Expr<float2>, Expr<float>> importance_sample_pixel_position(Expr<uint2> p, Expr<float2> u) = 0;
    };
}