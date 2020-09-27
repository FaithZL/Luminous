//
// Created by Zero on 2020/9/27.
//

#pragma once

#include <compute/dsl.h>

namespace luminous::compute {
    struct alignas(8) ClosestHit {
        float distance;
        uint triangle_id;
        uint instance_id;
        float2 bary;
    };

    struct AnyHit {
        float distance;
    };
}

LUMINOUS_STRUCT(luminous::compute::ClosestHit, distance, triangle_id, instance_id, bary)
LUMINOUS_STRUCT(luminous::compute::AnyHit, distance)