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
        _resolution(params.get("resolution").as_uint2(make_uint2(500, 500))) {

        }

        [[nodiscard]] uint2 resolution() const noexcept { return _resolution; }

        [[nodiscard]] auto clear() {
            return [this](Pipeline &pipeline) { _clear(pipeline); };
        }

        [[nodiscard]] auto accumulate_frame(const BufferView<float3> &radiance_buffer, const BufferView<float> &weight_buffer) {
            return [this, &radiance_buffer, &weight_buffer](Pipeline &pipeline) {
                _accumulate_frame(pipeline, radiance_buffer, weight_buffer);
            };
        }

        [[nodiscard]] auto postprocess() {
            return [this](Pipeline &pipeline) { _postprocess(pipeline); };
        }

        [[nodiscard]] auto save(std::filesystem::path path) {
            return [this, path = std::move(path)](Pipeline &pipeline) { _save(pipeline, path); };
        }
    };

}