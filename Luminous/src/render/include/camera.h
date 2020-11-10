//
// Created by Zero on 2020/10/20.
//

#pragma once

#include <render/plugin.h>
#include <compute/dsl.h>
#include <compute/pipeline.h>

namespace luminous::render {
    using compute::Ray;

    struct RaySample {
        Ray ray;
        float3 throughput;
    };
}

LUMINOUS_STRUCT(luminous::render::RaySample, ray, throughput);

namespace luminous::render {
    using compute::Pipeline;
    using compute::BufferView;
    using compute::TextureView;
    using compute::dsl::Expr;
    using compute::dsl::Var;

    class Camera : public Plugin {
    private:
        std::shared_ptr<Film> _film;
        std::shared_ptr<Filter> _filter;
        std::shared_ptr<Transform> _transform;

        BufferView<float2> _pixel_position_buffer;
        BufferView<Ray> _camera_ray_buffer;
        BufferView<float3> _throughput_buffer;
        BufferView<float> _pixel_weight_buffer;

        float4x4 _o2w;
    public:
        Camera(Device *d, const ParamSet &params)
                : Plugin{d, params} {

        }
    };
}

