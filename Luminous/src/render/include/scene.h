//
// Created by Zero on 2020/11/28.
//

#pragma once

#include <compute/device.h>
#include <compute/dispatcher.h>
#include <compute/buffer.h>
#include <compute/pipeline.h>
#include <compute/acceleration.h>

namespace luminous::render {
    struct LightSelection {
        uint index;
        float pdf;
    };

    struct LightSample {
        float3 wi;
        float3 Li;
        float pdf;
        float distance;
    };
}

LUMINOUS_STRUCT(luminous::render::LightSelection, index, pdf)
LUMINOUS_STRUCT(luminous::render::LightSample, wi, Li, pdf, distance)

namespace luminous::render {
    using compute::Device;
    using compute::BufferView;
    using compute::KernelView;
    using compute::Pipeline;
    using compute::Acceleration;

    using compute::Ray;
    using compute::AnyHit;
    using compute::ClosestHit;
    using compute::MeshHandle;

    class Scene {

    };
}