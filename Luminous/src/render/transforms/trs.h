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

        /**
         * @param data : {
         *      "type" : "translate",
         *      "param" : [1,0,1]
         * }
         */
        void parse(const ParamSet &params) noexcept {
            if (params["type"] == "translation") {
                parse_translation(params["param"]);
            } else if (params["type"] == "rotate") {
                parse_rotate(params["param"]);
            } else if (params["type"] == "scale") {
                parse_scaling(params["param"]);
            } else if (params["type"] == "matrix") {
                parse_matrix(params["param"]);
            } else {
                parse_default();
            }
        }

        void parse_translation(const ParamSet &data) noexcept {
            _t = make_float3(data);
            _matrix = luminous::math::translation(_t);
        }

        void parse_rotate(const ParamSet &data) noexcept {

        }

        void parse_default() noexcept {
            _matrix = luminous::math::identity();
        }

        void parse_scaling(const ParamSet &data) noexcept {

        }

        void parse_matrix(const ParamSet &data) noexcept {

        }

        void decompose(const float4x4 &matrix) noexcept {
            // extract translation component
            _t = make_float3(matrix[3]);


        }

    public:
        TRSTransform(Device * device, const ParamSet &params);
        [[nodiscard]] float4x4 matrix(float time) const noexcept override { return _matrix; }
        [[nodiscard]] auto translation() const noexcept { return _t; }
        [[nodiscard]] auto rotation() const noexcept { return _r; }
        [[nodiscard]] auto scaling() const noexcept { return _s; }
        [[nodiscard]] bool is_static() const noexcept override { return true; }
    };
}