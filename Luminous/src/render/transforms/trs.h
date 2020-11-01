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
        void parse(const ParamSet &data) noexcept {
            if (data.get("type") == "translation") {
                parse_translation(data.get("param"));
            } else if (data.get("type") == "rotate") {
                parse_rotate(data.get("param"));
            } else if (data.get("type") == "scale") {
                parse_scaling(data.get("param"));
            } else if (data.get("type") == "matrix") {
                parse_matrix(data.get("param"));
            } else {
                parse_default();
            }
            decompose(_matrix);
        }

        void parse_translation(const ParamSet &data) noexcept {
            auto t = data.as_float3();
            _matrix = luminous::math::translation(t);
        }

        void parse_rotate(const ParamSet &data) noexcept {
            auto r = data.as_float4();
            _matrix = luminous::math::rotation(make_float3(r), r.w);
        }

        void parse_scaling(const ParamSet &data) noexcept {
            auto s = data.as_float3(make_float3(1));
            _matrix = luminous::math::scaling(s);
        }

        void parse_default() noexcept {
            _matrix = make_float4x4(1.0f);
        }

        void parse_matrix(const ParamSet &data) noexcept {
            _matrix = data.as_float4x4();
        }

        void decompose(const float4x4 &matrix) noexcept {
            // extract translation component
            _t = make_float3(matrix[3]);

            auto M = make_float4x4();
            float norm = 0;
            int count = 0;
            auto R = M;
            do {
                // 计算下一个矩阵
                float4x4 R_next;
                float4x4 Rit = inverse(transpose(R));
                R_next = 0.5f * (R + Rit);

                // 对比两个矩阵的差异，如果差异小于0.0001则分解完成
                norm = 0;
                for (int i = 0; i < 3; ++i) {
                    float n = std::abs(R[i][0] - R_next[i][0]) +
                              std::abs(R[i][1] - R_next[i][1]) +
                              std::abs(R[i][2] - R_next[i][2]);
                    norm = std::max(norm, n);
                }
                R = R_next;
            } while (++count < 100 && norm > .0001);


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