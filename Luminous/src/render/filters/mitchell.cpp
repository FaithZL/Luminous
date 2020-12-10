//
// Created by Zero on 2020/12/10.
//

#include <render/include/filter.h>

namespace luminous::render::filter {

    class MitchellNetravaliFilter : public SeparableFilter {

    private:
        float _b;
        float _c;

    private:
        [[nodiscard]] float _weight_1d(float offset) const noexcept override {
            auto x = std::min(std::abs(2.0f * offset / radius()), 2.0f);
            return (x > 1.0f ?
                    (-_b - 6.0f * _c) * x * x * x + (6.0f * _b + 30.0f * _c) * x * x + (-12.0f * _b - 48.0f * _c) * x + (8.0f * _b + 24.0f * _c) :
                    (12.0f - 9.0f * _b - 6.0f * _c) * x * x * x + (-18.0f + 12.0f * _b + 6.0f * _c) * x * x + (6.0f - 2.0f * _b)) *
                   (1.0f / 6.0f);
        }

    public:
        MitchellNetravaliFilter(Device *device, const ParamSet &parameters)
                : SeparableFilter{device, parameters},
                  _b{parameters["b"].as_float(1.0f / 3.0f)},
                  _c{parameters["c"].as_float(1.0f / 3.0f)} {}
    };

}

LUMINOUS_EXPORT_PLUGIN_CREATOR(luminous::render::filter::MitchellNetravaliFilter)