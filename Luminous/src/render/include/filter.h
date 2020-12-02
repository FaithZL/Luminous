//
// Created by Zero on 2020/10/17.
//

#pragma once

#include <render/plugin.h>
#include <compute/dsl.h>
#include <compute/pipeline.h>
#include <render/include/param_set.h>

namespace luminous::render {
    struct FilterSample {
        float2 p;
        float weight;
    };
}

LUMINOUS_STRUCT(luminous::render::FilterSample, p, weight)

namespace luminous::render {

    using luminous::compute::Device;
    using luminous::compute::dsl::Expr;

    using compute::Device;
    using compute::Pipeline;
    using compute::KernelView;
    using compute::dsl::Var;
    using compute::dsl::Expr;


    class Filter : public Plugin {
    private:
        float _radius;

        [[nodiscard]] virtual Expr<FilterSample> _importance_sample_pixel_position(Expr<uint2> p, Expr<float2> u) = 0;

    public:
        Filter(Device *device, const ParamSet &params)
        : Plugin(device, params),
        _radius(params.get("radius").as_float(2)) {

        }

        [[nodiscard]] float radius() const noexcept {
            return _radius;
        }

        [[nodiscard]] Expr<FilterSample> importance_sample_pixel_position(Expr<uint2> p,
                                                                           Expr<float2> u) {
            return _importance_sample_pixel_position(p, u);
        }
    };

    class SeparableFilter : public Filter {

    public:
        static constexpr auto lookup_table_size = 64u;

    private:
        std::array<float, lookup_table_size> _weight_table{};
        std::array<float, lookup_table_size> _cdf_table{};
        float _scale{};
        bool _table_generated{false};

        // Filter 1D weight function, offset is in range [-radius, radius)
        [[nodiscard]] virtual float _weight_1d(float offset) const noexcept  = 0;

        // (position, weight)
        [[nodiscard]] Expr<FilterSample> _importance_sample_pixel_position(Expr<uint2> p, Expr<float2> u) override;

    public:
        SeparableFilter(Device *device, const ParamSet &params)
                : Filter{device, params} {}
    };
}