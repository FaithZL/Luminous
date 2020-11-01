//
// Created by Zero on 2020/10/30.
//

#include "trs.h"

namespace luminous::render::transform {

    TRSTransform::TRSTransform(Device * device, const ParamSet &data)
    : Transform(device, data),
    _t(make_float3(0.0f)),
    _r(make_float4(0.0f)),
    _s(make_float3(1.0f)) {
        _matrix = parse(data);
        decompose(_matrix);
    }

}

LUMINOUS_EXPORT_PLUGIN_CREATOR(luminous::render::transform::TRSTransform)