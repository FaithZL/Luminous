//
// Created by Zero on 2020/11/19.
//

#include "render/include/filter.h"

namespace luminous::render::filter {
    class GaussianFilter : public SeparableFilter {
    private:
        float _alpha;
    private:
        [[nodiscard]] float _weight_1d(float offset) const noexcept override {
            return std::max(0.0f, std::exp(-_alpha * offset * offset) - std::exp(-_alpha * radius() * radius()));
        }

    public:
        GaussianFilter(Device *device, const ParamSet &params)
        : SeparableFilter{device, params},
        _alpha(params.get("alpha").as_float(2.f)){

        }

    };
}

LUMINOUS_EXPORT_PLUGIN_CREATOR(luminous::render::filter::GaussianFilter)