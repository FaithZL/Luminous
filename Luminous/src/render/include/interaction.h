//
// Created by Zero on 2020/12/13.
//

#pragma once

#include <optional>

#include <compute/buffer.h>
#include <compute/device.h>
#include <compute/dsl.h>

namespace luminous::render {
    struct ShaderSelection {
        uint type;
        uint index;
        float prob;
        float weight;
    };

    struct Interaction {

        enum Component : uint {
            COMPONENT_MISS = 1u << 0u,
            COMPONENT_PI = 1u << 1u,
            COMPONENT_WO = 1u << 2u,
            COMPONENT_NG = 1u << 3u,
            COMPONENT_NS = 1u << 4u,
            COMPONENT_UV = 1u << 5u,
            COMPONENT_DISTANCE = 1u << 6u,
            COMPONENT_SHADER = 1u << 7u,
            COMPONENT_PDF = 1u << 8u,

            COMPONENT_NONE = 0u,
            COMPONENT_ALL = 0xffffffffu
        };

        bool miss;
        float3 pi;
        float3 wo;
        float distance;
        float3 ng;
        float3 ns;
        float2 uv;
        float pdf;
        ShaderSelection shader;
    };
}

LUMINOUS_STRUCT(luminous::render::ShaderSelection, type, index, prob, weight)
LUMINOUS_STRUCT(luminous::render::Interaction, miss, pi, wo, distance, ng, ns, uv, pdf, shader)