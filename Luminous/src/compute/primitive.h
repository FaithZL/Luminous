//
// Created by Zero on 2020/9/27.
//

#pragma once

#include "dsl_syntax.h"

namespace luminous::compute {

    struct Vertex {
        float3 position;
        float3 normal;
        float2 uv;
    };

}

LUMINOUS_STRUCT(Vertex, position, normal, uv);