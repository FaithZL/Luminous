//
// Created by Zero on 2020/10/20.
//

#pragma once

#include <render/plugin.h>
#include <compute/dsl.h>
#include <compute/pipeline.h>
#include "transform.h"

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

        [[nodiscard]] Film *film() const noexcept { return _film.get(); }
        [[nodiscard]] Filter *filter() const noexcept { return _filter.get(); }
        [[nodiscard]] Transform *transform() const noexcept { return _transform.get(); }

        [[nodiscard]] const BufferView<float2> &pixel_position_buffer() const noexcept { return _pixel_position_buffer; }
        [[nodiscard]] const BufferView<float> &pixel_weight_buffer() const noexcept { return _pixel_weight_buffer; }
        [[nodiscard]] BufferView<Ray> &ray_buffer() noexcept { return _camera_ray_buffer; }
        [[nodiscard]] BufferView<float3> &throughput_buffer() noexcept { return _throughput_buffer; }

        [[nodiscard]] std::function<void(Pipeline &pipeline)> generate_rays(float time, Sampler &sampler);
        [[nodiscard]] bool is_static() const noexcept { return _transform == nullptr || _transform->is_static(); }
    };
}

