//
// Created by Zero on 2020/10/17.
//

#include <render/include/sampler.h>
#include <compute/dsl.h>

namespace luminous::render::sampler {

    using namespace compute;
    using namespace compute::dsl;

    class IndependentSampler : public Sampler {

    private:
        BufferView<uint> _state_buffer;
        uint _num_dims{};

    private:
        [[nodiscard]] static Expr<float> _rnd(Expr<uint> prev) noexcept {
            constexpr auto lcg_a = 1664525u;
            constexpr auto lcg_c = 1013904223u;
            prev = (lcg_a * prev + lcg_c);
            return cast<float>(prev & 0x00ffffffu) * (1.0f / static_cast<float>(0x01000000u));
        };

        void _prepare_for_next_frame(Pipeline &pipeline) override {
            /* Nothing to do */
        }

        void _reset(Pipeline &pipeline, uint2 resolution) override {

            constexpr auto threadgroup_size = make_uint2(16u, 16u);
            auto pixel_count = resolution.x * resolution.y;

            if (_state_buffer.size() < pixel_count) {
                _state_buffer = device()->allocate_buffer<uint>(pixel_count);
            }

            pipeline << device()->compile_kernel("independent_sampler_reset", [&] {
                auto tea = [](Expr<uint2> val_in) -> Expr<uint> {
                    Var val = val_in;
                    Var v0 = val.x;
                    Var v1 = val.y;
                    Var s0 = 0u;
                    for (auto n = 0u; n < 5u; n++) {
                        s0 += 0x9e3779b9u;
                        v0 += ((v1 << 4u) + 0xa341316cu) ^ (v1 + s0) ^ ((v1 >> 5u) + 0xc8013ea4u);
                        v1 += ((v0 << 4u) + 0xad90777du) ^ (v0 + s0) ^ ((v0 >> 5u) + 0x7e95761eu);
                    }
                    return v0;
                };
                auto txy = thread_xy();
                If (all(resolution % threadgroup_size == make_uint2(0u)) || all(txy < resolution)) {
                    auto index = txy.y * resolution.x + txy.x;
                    _state_buffer[index] = tea(txy);
                };
            }).parallelize(resolution, threadgroup_size);
        }
    public:
        IndependentSampler(Device *d, const ParamSet &params)
                : Sampler{d, params} {}

        Expr<float> generate_1d_sample(Expr<uint> pixel_index) override {
            Var seed = _state_buffer[pixel_index];
            Var x = _rnd(seed);
            _state_buffer[pixel_index] = seed;
            return x;
        }

        Expr<float2> generate_2d_sample(Expr<uint> pixel_index) override {
            Var seed = _state_buffer[pixel_index];
            Var x = _rnd(seed);
            Var y = _rnd(seed);
            _state_buffer[pixel_index] = seed;
            return make_float2(x, y);
        }

        Expr<float3> generate_3d_sample(Expr<uint> pixel_index) override {
            Var seed = _state_buffer[pixel_index];
            Var x = _rnd(seed);
            Var y = _rnd(seed);
            Var z = _rnd(seed);
            _state_buffer[pixel_index] = seed;
            return make_float3(x, y, z);
        }

        Expr<float4> generate_4d_sample(Expr<uint> pixel_index) override {
            Var seed = _state_buffer[pixel_index];
            Var x = _rnd(seed);
            Var y = _rnd(seed);
            Var z = _rnd(seed);
            Var w = _rnd(seed);
            _state_buffer[pixel_index] = seed;
            return make_float4(x, y, z, w);
        }
    };
}

LUMINOUS_EXPORT_PLUGIN_CREATOR(luminous::render::sampler::IndependentSampler)