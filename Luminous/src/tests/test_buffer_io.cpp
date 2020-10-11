//
// Created by Zero on 2020/10/7.
//

#include <algorithm>
#include <random>
#include <vector>

#include <compute/device.h>
#include <compute/dsl_syntax.h>
#include <cxxopts.hpp>

using namespace luminous;
using namespace luminous::compute;
using namespace luminous::compute::dsl;
using namespace std;

int main(int argc, char *argv[]) {
    Context context{argc, argv};
    auto device = Device::create(&context);

    constexpr auto buffer_size = 1024u;

    auto buffer_a = device->allocate_buffer<float>(buffer_size);
    auto buffer_b = device->allocate_buffer<float>(buffer_size);

    std::vector<float> input(buffer_size);
    std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<float> distribution{0.0f, 1.0f};
    for (auto &&f : input) { f = distribution(rng); }

    auto input_copy = input;
    std::shuffle(input_copy.begin(), input_copy.end(), rng);

    auto scale_value = [&](Expr<float> k, Expr<float> x) -> Expr<float> {
        return k * x;
    };

    auto scale = 1.0f;
    auto kernel = device->compile_kernel("simple_test", [&] {
        auto tid = thread_id();
        Var<std::array<float, 8>> a{1, 2, 3, 4, 5, 6, 7, 8};
        If (tid < buffer_size) {
            buffer_b[tid] = scale_value(uniform(&scale), buffer_a[tid]);
        };
    });

    auto launch_index = 0u;

    for (auto i = 0; i < 20; i++) {
        device->launch([&](Dispatcher &dispatch) {
            scale = 3.0f;
            dispatch(buffer_a.copy_from(input_copy.data()));
            dispatch(kernel.parallelize(buffer_size), [&] { LUMINOUS_INFO("Kernel #", launch_index++, " finished."); });
        });
    }

    std::vector<float> output(buffer_size);
    device->launch([&](Dispatcher &d) {
        scale = 2.0f;
        d(buffer_a.copy_from(input.data()));
        d(kernel.parallelize(buffer_size), [&] { LUMINOUS_INFO("Kernel #", launch_index++, " finished."); });
        d(buffer_b.copy_to(output.data()));
    });

    for (auto i = 0; i < 20; i++) {
        device->launch([&](Dispatcher &d) {
            scale = 3.0f;
            d(buffer_a.copy_from(input_copy.data()));
            d(kernel.parallelize(buffer_size), [&] { LUMINOUS_INFO("Kernel #", launch_index++, " finished."); });
        });
    }

    device->synchronize();

    LUMINOUS_INFO("Done.");
    for (auto i = 0; i < buffer_size; i++) {
        if (output[i] != input[i] * 2.0f) {
            LUMINOUS_WARNING("Fuck! Expected ", input[i] * 2.0f, ", got ", output[i]);
        }
    }
    LUMINOUS_INFO("Results checked.");

    return 0;
}