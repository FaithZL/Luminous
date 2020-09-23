//
// Created by Zero on 2020/9/20.
//

#include <compute/dsl.h>
#include <compute/codegen.h>
#include <compute/buffer.h>
#include "compute/device.h"
#include "compute/function.h"
#include "compute/codegen.h"

using namespace std;

using namespace luminous;

using namespace luminous::compute;

using namespace luminous::compute::dsl;

template<typename Def, std::enable_if_t<std::is_invocable_v<Def>, int> = 0>
void fake_compile_kernel(std::string name, Def &&def) {
    LUMINOUS_INFO("Compiling kernel: ", name);
    Function f{std::move(name)};
    def();
    CppCodegen codegen{std::cout};
    codegen.emit(f);
    LUMINOUS_INFO("Done.");
}

int main() {

    fake_compile_kernel("test", [](){

    });

    return 0;
}
