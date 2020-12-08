//
// Created by Zero on 2020/11/22.
//

#include "render/include/film.h"

namespace luminous::render::film {

    using namespace compute;
    using namespace compute::dsl;

    class RGBFilm : public Film {
    private:
        TextureView _framebuffer;
    private:
        void _clear(Pipeline &pipeline) override {
            constexpr auto threadgroup_size = make_uint2(16u, 16u);
            pipeline << device()->compile_kernel("rgb_film_clear", [&] {
                auto txy = thread_xy();
                If (all(resolution() % threadgroup_size == make_uint2(0u)) || all(txy < resolution())) {
                    _framebuffer.write(txy, dsl::make_float4(0.0f));
                };
            }).parallelize(resolution(), threadgroup_size);
        }

        void _accumulate_frame(Pipeline &pipeline, const BufferView<float3> &radiance_buffer, const BufferView<float> &weight_buffer) override {
            constexpr auto threadgroup_size = make_uint2(16u, 16u);
            pipeline << device()->compile_kernel("rgb_film_accumulate", [&] {
                auto txy = thread_xy();
                If (all(resolution() % threadgroup_size == make_uint2(0u)) || all(txy < resolution())) {
                    Var index = txy.y * resolution().x + txy.x;
                    Var radiance = radiance_buffer[index];
                    Var weight = weight_buffer[index];
                    Var accum = _framebuffer.read(txy);
                    _framebuffer.write(txy, accum + make_float4(radiance * weight, weight));
                };
            }).parallelize(resolution(), threadgroup_size);
        }

        void _postprocess(Pipeline &pipeline) override {
            constexpr auto threadgroup_size = make_uint2(16u, 16u);
            pipeline << device()->compile_kernel("rgb_film_postprocess", [&] {
                auto txy = thread_xy();
                If (all(resolution() % threadgroup_size == make_uint2(0u)) || all(txy < resolution())) {
                    Var accum = _framebuffer.read(txy);
                    _framebuffer.write(txy, make_float4(
                            select(accum.w == 0.0f,
                                   dsl::make_float3(0.0f),
                                   make_float3(accum) / accum.w),
                            1.0f));
                };
            }).parallelize(resolution(), threadgroup_size);
        }

        void _save(Pipeline &pipeline, const std::filesystem::path &path) override {
            pipeline << _framebuffer.save(path);
        }

    public:
        RGBFilm(Device *d, const ParamSet &params) : Film{d, params} {
            _framebuffer = device()->allocate_texture<float4>(resolution().x, resolution().y);
        }
    };
}

LUMINOUS_EXPORT_PLUGIN_CREATOR(luminous::render::film::RGBFilm)