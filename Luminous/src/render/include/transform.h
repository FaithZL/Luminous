//
// Created by Zero on 2020/10/15.
//

#pragma once

#include <algorithm>
#include <vector>
#include <memory>

#include <compute/dsl.h>
#include <render/plugin.h>

namespace luminous::render {
    
    class Transform : public Plugin {
    public:
        Transform(Device *device, const ParamSet &params) noexcept :
        Plugin(device, params) {

        }

        [[nodiscard]] virtual bool is_static() const noexcept = 0;
        [[nodiscard]] virtual float4x4 matrix(float time) const noexcept = 0;
    };

}