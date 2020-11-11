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
            _val = value.as<T>();
        }

        [[nodiscard]] T eval(const float2 &uv) const override {
            return _val;
        }

        [[nodiscard]] T eval_gradient(const float2 &uv) const override {
            return T(0.0f);
        }
    };
}


#ifdef FLOAT3
    LUMINOUS_EXPORT_PLUGIN_CREATOR(luminous::render::texture::ConstantTexture<luminous::float3>)
#endif

#ifdef FLOAT4
    LUMINOUS_EXPORT_PLUGIN_CREATOR(luminous::render::texture::ConstantTexture<luminous::float4>)
#endif