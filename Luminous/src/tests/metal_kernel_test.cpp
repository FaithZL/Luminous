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
    context.add_cli_option<uint>("b,blocksize", "Block size (results are sorted block-wise)", "1024");
    context.try_print_help_and_exit();

    auto device = Device::create(&context);
    static constexpr auto buffer_size = 1024u * 1024u;
    static constexpr auto block_size = 1024u;

    std::vector<float> host_buffer(buffer_size);
    for (auto i = 0u; i < buffer_size; i++) {
        host_buffer[i] = static_cast<float>(i);
    }
    auto buffer = device->allocate_buffer<float>(buffer_size);

    auto stride = 0u;
    auto step = 0u;

//    auto kernel = device->compile_kernel("basic", [&] {
//
//        auto cmp_stride_in = uniform(&stride);
//        auto cmp_step_in = uniform(&step);
//
//        Var cmp_step = cmp_step_in;
//        Var half_cmp_step = cmp_step / 2u;
//        Var tid = thread_id();
//        Var lhs_index = tid / half_cmp_step * cmp_step + tid % half_cmp_step;
//        Var rhs_index = lhs_index + half_cmp_step;
//
//        Var lhs = buffer[lhs_index];
//        Var rhs = buffer[rhs_index];
//
//        Var predicate = cast<bool>(lhs_index & cmp_stride_in) ^ (lhs < rhs);
//        buffer[lhs_index] = select(predicate, lhs, rhs);
//        buffer[rhs_index] = select(predicate, rhs, lhs);
//    });

    return 0;
}