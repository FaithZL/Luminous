//
// Created by Zero on 2020/10/17.
//

#pragma once

#include <compute/dsl_syntax.h>
//#include <core/math/math_util.h>

namespace luminous::render {
    inline namespace sampling {

        using compute::dsl::Var;
        using compute::dsl::Expr;

        /**
         * 均匀分布可得p(x,y) = 1/π
         * p(x,y) = p(θ,r)/r 8式
         * 又由8式，可得
         * p(θ,r) = r/π
         * 由边缘概率密度函数公式可得
         * p(r) = ∫[0,2π]p(θ,r)dθ = 2r
         * p(θ|r) = p(θ,r)/p(r) = 1/2π
         * θ与r相互独立 p(θ|r) = 1/2π = p(θ)
         * 对p(θ)积分可得
         * P(θ) = θ/2π
         * 对p(r)积分可得
         * P(r) = r^2
         *
         * a,b为[0,1]的均匀分布随机数
         * r = √(a)
         * θ = 2πb
         */
        inline Expr<float2> uniform_sample_disk(Expr<float2> u) {
            Var r = sqrt(u.x);
            Var theta = constant::_2Pi * u.y;
            return make_float2(r * cos(theta), r * sin(theta));
        }

        inline Expr<float3> cosine_sample_hemisphere(Expr<float2> u) {
            Expr<float2> d = uniform_sample_disk(u);
            Var z = sqrt(max(0.0f, 1.0f - d.x * d.x - d.y * d.y));
            return make_float3(d.x, d.y, z);
        }

        /**
         * p(θ, φ) = sinθ p(w)
         * p(θ, φ) = pθ(θ) * pφ(φ)
         * pφ(φ) = 1/2π
         *
         * p(w) = pθ(θ)/(2π * sinθ) 为常数
         * 所以p(θ)/sinθ为常数
         * 假设p(θ) = c * sinθ
         *
         * 1 = ∫[0,θmax]p(θ)dθ
         * 求得 c = 1/(1 - cosθmax)
         * p(θ) = sinθ/(1 - cosθmax)
         * p(w) = p(θ, φ)/sinθ = pθ(θ) * pφ(φ)/sinθ = 1/(2π(1-cosθmax))
         */
         inline Expr<float> uniform_cone_pdf(Expr<float> cos_theta_max) {
            return 1 / (constant::_2Pi * (1 - cos_theta_max));
         }

    }
}