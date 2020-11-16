//
// Created by Zero on 2020/10/17.
//

#include "render/include/filter.h"

namespace luminous::render::filter {
    class BoxFilter : public SeparableFilter {
    private:
        [[nodiscard]] float _weight_1d(float offset) const noexcept override {
            return 1.0f;
        }

    public:
        BoxFilter(Device *device, const ParamSet &params) : SeparableFilter{device, params} {}
    };
}

LUMINOUS_EXPORT_PLUGIN_CREATOR(luminous::render::filter::BoxFilter);