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
        float2 uv2;
    };

    struct TriangleHandle {
        uint i;
        uint j;
        uint k;
    };

    struct EntityHandle {
        uint vertex_offset;
        uint triangle_offset;
    };

    struct MeshHandle : public EntityHandle {
        uint vertex_count;
        uint triangle_count;
    };

}

LUMINOUS_STRUCT(luminous::compute::Vertex, position, normal, uv);
LUMINOUS_STRUCT(luminous::compute::TriangleHandle, i, j, k);
LUMINOUS_STRUCT(luminous::compute::EntityHandle, vertex_offset, triangle_offset);
LUMINOUS_STRUCT(luminous::compute::MeshHandle, vertex_offset, triangle_offset, vertex_count, triangle_count);