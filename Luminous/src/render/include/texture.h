//
// Created by Zero on 2020/11/5.
//

#pragma once

#include <render/plugin.h>
#include <compute/dsl.h>
#include "compute/pipeline.h"
#include <render/include/param_set.h>
#include <map>

namespace luminous::render {
    template <typename T>
    class Texture : public Plugin {
    public:
        Texture(Device *device, const ParamSet &params)
        : Plugin(device, params) {

        }

        [[nodiscard]] virtual T eval(const float2 &uv) const = 0;

        [[nodiscard]] virtual T eval_gradient(const float2 &uv) const = 0;
    };
}