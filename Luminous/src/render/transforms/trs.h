//
// Created by Zero on 2020/10/30.
//

#pragma once

#include "render/include/transform.h"

namespace luminous::render::transform {
    class TRSTransform : public Transform {
    protected:
        float3 _t;
        float4 _r;
        float3 _s;
        float4x4 _matrix{};

        //{
        //    "type" : "translate",
        //    "param" : [1,0,1]
        //},
        void parse(const nloJson &params) noexcept {
            if (params["type"] == "translation") {
                parse_translation(params["param"]);
            } else if (params["type"] == "rotate") {
                parse_rotate(params["param"]);
            } else if (params["type"] == "scale") {
                parse_scaling(params["param"]);
            }
        }

        void parse_translation(const nloJson &data) noexcept {

        }

        void parse_rotate(const nloJson &data) noexcept {

        }

        void parse_scaling(const nloJson &data) noexcept {

        }



    public:
        TRSTransform(Device * device, const nloJson &params);
        [[nodiscard]] float4x4 matrix(float time) const noexcept override { return _matrix; }
        [[nodiscard]] auto translation() const noexcept { return _t; }
        [[nodiscard]] auto rotation() const noexcept { return _r; }
        [[nodiscard]] auto scaling() const noexcept { return _s; }
        [[nodiscard]] bool is_static() const noexcept override { return true; }
    };
}