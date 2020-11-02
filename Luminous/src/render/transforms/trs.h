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
        [[nodiscard]] float4x4 parse(const ParamSet &data) noexcept {
            if (data.json().is_array()) {
                float4x4 ret = make_float4x4(1.0f);
                for (int i = 0; i < data.json().size(); ++i) {
                    ret = ret * parse(data.json()[i]);
                }
                return ret;
            }
            if (data.get("type").json() == "translation") {
                return parse_translation(data.get("param"));
            } else if (data.get("type").json() == "rotate") {
                return parse_rotate(data.get("param"));
            } else if (data.get("type").json() == "scale") {
                return parse_scaling(data.get("param"));
            } else if (data.get("type").json() == "matrix") {
                return parse_matrix(data.get("param"));
            } else {
                return parse_default();
            }
        }

        [[nodiscard]] float4x4 parse_translation(const ParamSet &data) noexcept {
            auto t = data.as_float3();
            return luminous::math::translation(t);
        }

        [[nodiscard]] float4x4 parse_rotate(const ParamSet &data) noexcept {
            auto r = data.as_float4();
            return luminous::math::rotation(make_float3(r), r.w);
        }

        [[nodiscard]] float4x4 parse_scaling(const ParamSet &data) noexcept {
            auto s = data.as_float3(make_float3(1));
            return luminous::math::scaling(s);
        }

        [[nodiscard]] float4x4 parse_default() noexcept {
            return make_float4x4(1.0f);
        }

        [[nodiscard]] float4x4 parse_matrix(const ParamSet &data) noexcept {
            return data.as_float4x4();
        }

        void decompose(const float4x4 &matrix) noexcept {

            auto M = matrix;
            for (int i = 0; i < 3; ++i) {
                M[i][3] = M[3][i] = 0.0f;
            }
            M[3][3] = 1.f;

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

            float4x4 S = M * inverse(R);

            // extract translation component
            _t = make_float3(matrix[3]);
            _r = matrix_to_quaternion(R);
            _s = make_float3(S[0][0], S[1][1], S[2][2]);
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
