//
// Created by Zero on 2020/10/19.
//

#pragma once

#include <render/plugin.h>
#include <compute/dsl.h>
#include "render/include/transform.h"
#include "render/include/scene.h"
#include "interaction.h"
#include "shape.h"

namespace luminous::render {
    using luminous::compute::Device;
    using luminous::compute::dsl::Expr;

    class Light : public Plugin {
    private:
        shared_ptr<Transform> _o2w;
    public:

        Light(Device *device, const ParamSet &params) noexcept
                : Plugin{device, params} {

        }

        virtual void preprocess(const Scene &scene) {

        }

        virtual Expr<float3> sample_Li(Expr<Interaction>, Expr<float3> u, const Scene &scene) const = 0;

        virtual Expr<float> pdf_Li(Expr<Interaction>, Expr<float3> u, const Scene &scene) const = 0;

        virtual Expr<float> power() = 0;
    };

    class AreaLight : public Light {
    private:
        std::shared_ptr<Shape> _shape;
    public:
        AreaLight(Device *device, const ParamSet &params) noexcept
        : Light{device, params} {

        }

        virtual Expr<float> surface_area() const = 0;

        virtual Expr<float3> L(Expr<Interaction>, Expr<float3> wi) const = 0;
    };
}