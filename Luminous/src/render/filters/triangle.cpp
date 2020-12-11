//
// Created by Zero on 2020/11/16.
//

#include "render/include/filter.h"

namespace luminous::render::filter {
    class TriangleFilter : public SeparableFilter {
    private:
        [[nodiscard]] float _weight_1d(float offset) const noexcept override {
            return std::max(0.0f, radius() - std::abs(offset));
        }

    public:
        TriangleFilter(Device *device, const ParamSet &params) :
        SeparableFilter{device, params} {

        }

    };
}

LUMINOUS_EXPORT_PLUGIN_CREATOR(luminous::render::filter::TriangleFilter)