//
// Created by Zero on 2020/11/5.
//

#pragma once

#include <render/plugin.h>
#include <compute/dsl.h>
#include "compute/pipeline.h"
#include <map>
#include "texture.h"

namespace luminous::render {
    using luminous::compute::Device;
    using luminous::compute::dsl::Expr;

    class Rasterizer : public Plugin {

    private:
        std::map<string_view, float *> _texture;

    public:
        Rasterizer(Device *device, const ParamSet &params)
        : Plugin(device, params) {

        }

        virtual void set_texture(const string_view key) = 0;

    };
}