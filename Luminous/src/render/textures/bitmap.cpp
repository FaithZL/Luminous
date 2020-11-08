//
// Created by Zero on 2020/11/7.
//

#include "render/include/texture.h"
#include <compute/dsl.h>

namespace luminous::render::texture {

    using namespace compute;
    using namespace compute::dsl;

    template <typename T>
    class BitmapTexture : public Texture<T> {

    };
}