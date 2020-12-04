//
// Created by Zero on 2020/12/4.
//


#include "render/include/model.h"
#include <random>
#include <compute/device.h>
#include <compute/dsl_syntax.h>

using namespace luminous;
using namespace luminous::compute;
using namespace luminous::compute::dsl;

using luminous::render::MeshesCache;

int main(int argc, char *argv[]) {
    Context context{argc, argv};
    auto device = Device::create(&context);
    auto meshes = MeshesCache::get_meshes("/Users/zero/work/graphic/renderer/Luminous/Luminous/res/models/teapot.obj", 0);
    return 0;
}