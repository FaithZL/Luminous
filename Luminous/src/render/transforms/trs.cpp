//
// Created by Zero on 2020/10/30.
//

#include "trs.h"

namespace luminous::render::transform {

    TRSTransform::TRSTransform(Device * device, const nloJson &params)
    : Transform(device, params) {
        parse(params);
    }

}

LUMINOUS_EXPORT_PLUGIN_CREATOR(luminous::render::transform::TRSTransform)