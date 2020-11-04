//
// Created by Zero on 2020/11/4.
//


#include "util/parallel.h"

using namespace luminous;
using namespace std;

int main(int argc, char *argv[]) {
    auto func = [](uint32_t i, uint32_t j) {
        LUMINOUS_INFO("i = ", i, ",j = ", j);
    };
    parallel_for(10,func , 2);

    auto func_2d = [](const int2 &dim, u_int32_t tid) {
        LUMINOUS_INFO(dim.to_string());
    };

    parallel_for_2d(make_int2(5,5), func_2d);

    return 0;
}