//
// Created by Zero on 2020/12/11.
//

#include "render/include/filter.h"

namespace luminous::render::filter {
    class LanczosSincFilter : public SeparableFilter {

    private:
        float _tau;

    private:
        [[nodiscard]] float _weight_1d(float offset) const noexcept override {
            constexpr auto sinc = [](float x) noexcept {
                x = std::abs(x);
                return x < 1e-5f ? 1.0f : std::sin(math::Pi * x) / (math::Pi * x);
            };
            auto x = std::abs(offset);
            return x > radius() ? 0.0f : sinc(x) * sinc(x / _tau);
        }

    public:
        LanczosSincFilter(Device *device, const ParamSet &params)
                : SeparableFilter{device, params},
                  _tau{params["tau"].as_float(3.0f)} {}
    };
}

LUMINOUS_EXPORT_PLUGIN_CREATOR(luminous::render::filter::LanczosSincFilter)