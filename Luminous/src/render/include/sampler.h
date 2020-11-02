//
// Created by Zero on 2020/10/17.
//

#pragma once

#include <render/plugin.h>
#include <compute/dsl.h>
#include <core/header.h>
#include "compute/pipeline.h"

namespace luminous::render {
    using luminous::compute::Device;
    using luminous::compute::dsl::Expr;
    using luminous::compute::Pipeline;

    class Sampler : public Plugin {
    protected:
        uint _spp;
        uint _cur_frame_idx;

    private:
        virtual void _prepare_for_next_frame(Pipeline &pipeline) = 0;
        virtual void _reset(Pipeline &pipeline, uint2 resolution) = 0;
    public:
        Sampler(Device * device, const ParamSet &param)
        : Plugin(device, param),
        _spp(param.get("spp").as_uint(128)) {

        }

        [[nodiscard]] uint spp() const noexcept { return _spp; }

        [[nodiscard]] uint current_frame_index() const noexcept { return _cur_frame_idx; }

        [[nodiscard]] virtual Expr<float> generate_1d_sample(Expr<uint> pixel_index) = 0;

        [[nodiscard]] virtual Expr<float2> generate_2d_sample(Expr<uint> pixel_index) = 0;

        [[nodiscard]] virtual Expr<float3> generate_3d_sample(Expr<uint> pixel_index) = 0;

        [[nodiscard]] virtual Expr<float4> generate_4d_sample(Expr<uint> pixel_index) = 0;

        [[nodiscard]] auto prepare_for_next_frame() {
            return [this](Pipeline &pipeline) {
                pipeline << [this] {
                    _cur_frame_idx++;
                    LUMINOUS_WARNING_IF_NOT(_cur_frame_idx <= _spp,
                                            "Current frame index ",
                                            _cur_frame_idx,
                                            " exceeds samples per pixel: ",
                                            _spp);
                };
                _prepare_for_next_frame(pipeline);
            };
        }

        [[nodiscard]] auto reset(uint2 resolution) {
            return [this, resolution](Pipeline &pipeline) {
                pipeline << [this] { _cur_frame_idx = 0u; };
                _reset(pipeline, resolution);
            };
        }

    };
}