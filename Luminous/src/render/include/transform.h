//
// Created by Zero on 2020/10/15.
//

#pragma once

#include <algorithm>
#include <vector>
#include <memory>

#include <compute/dsl.h>
#include <render/plugin.h>
#include <render/include/param_set.h>

namespace luminous::render {
    
    class Transform : public Plugin {
    public:
        Transform(Device *device, const ParamSet &params) noexcept :
        Plugin(device, params) {

        }

        [[nodiscard]] virtual bool is_static() const noexcept = 0;
        [[nodiscard]] virtual float4x4 matrix(float time) const noexcept = 0;

        virtual float3 translation() const noexcept {
            LUMINOUS_EXCEPTION("translation is not implemented !")
        }
        virtual float4 rotation() const noexcept {
            LUMINOUS_EXCEPTION("rotation is not implemented !");
        }
        virtual float3 scaling() const noexcept {
            LUMINOUS_EXCEPTION("scaling is not implemented !")
        }
    };

}