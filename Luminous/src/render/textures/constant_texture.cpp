//
// Created by Zero on 2020/11/7.
//

#include "render/include/texture.h"
#include <compute/dsl.h>

namespace luminous::render::texture {
    using namespace compute;
    using namespace compute::dsl;

    template <typename T>
    class ConstantTexture : public Texture<T> {

    private:

        T _val;

    public:
        ConstantTexture(Device * device, const ParamSet &param)
        : Texture<T>(device, param) {
            auto value = param.get("value");

        }


        virtual T eval(const float2 &uv) = 0;

        virtual T eval_gradient(const float2 &uv) = 0;
    };
}