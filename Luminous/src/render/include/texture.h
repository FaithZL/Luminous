//
// Created by Zero on 2020/11/5.
//

#pragma once

#include <render/plugin.h>
#include <compute/dsl.h>
#include "compute/pipeline.h"
#include <map>

namespace luminous::render {
    template <typename T>
    class Texture : public Plugin {
    public:
        Texture(Device *device, const ParamSet &params)
        : Plugin(device, params) {

        }

        virtual T eval(const float2 &uv) = 0;
        
        virtual T eval_gradient(const float2 &uv) = 0;
    };
}