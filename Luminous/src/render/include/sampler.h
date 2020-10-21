//
// Created by Zero on 2020/10/17.
//

#pragma once

#include <render/plugin.h>
#include <compute/dsl.h>

namespace luminous::render {
    using luminous::compute::Device;
    using luminous::compute::dsl::Expr;

    class Sampler : public Plugin {
    protected:
        uint _spp;
        uint _cur_frame_idx;

    public:
        Sampler(Device * device, const nloJson &param)
        : Plugin(device, param),
        _spp(param.value("spp", 8)) {

        }

        [[nodiscard]] uint spp() const noexcept {
            return _spp;
        }

        [[nodiscard]] virtual Expr<float> generate_1d_sample(Expr<uint> pixel_index) = 0;

        [[nodiscard]] virtual Expr<float2> generate_2d_sample(Expr<uint> pixel_index) = 0;

        [[nodiscard]] virtual Expr<float3> generate_3d_sample(Expr<uint> pixel_index) = 0;

        [[nodiscard]] virtual Expr<float4> generate_4d_sample(Expr<uint> pixel_index) = 0;

        [[nodiscard]] auto reset(uint2 resolution) {

        }

    };
}