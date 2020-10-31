//
// Created by Zero on 2020/10/27.
//

#pragma once

#include <compute/pipeline.h>
#include <render/plugin.h>

namespace luminous::render {
    using compute::TextureView;
    using compute::BufferView;
    using compute::Pipeline;

    class Film : public Plugin {
    private:
        uint2 _resolution;

    private:
        virtual void _clear(Pipeline &pipeline) = 0;

        virtual void _accumulate_frame(Pipeline &pipeline,
                                       const BufferView<float3> &radiance_buffer,
                                       const BufferView<float> &weight_buffer) = 0;

        virtual void _postprocess(Pipeline &pipeline) = 0;

        virtual void _save(Pipeline &pipeline, const std::filesystem::path &path) = 0;

    public:
        Film(Device *device, const ParamSet &params)
        :Plugin(device, params),
        _resolution(params.value("resolution",
                                 ParamSet::array({500, 500}))) {

        }
    };

}