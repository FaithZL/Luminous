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

    return 0;
}