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
    Function::push(&f);
    def();
    CppCodegen codegen{std::cout};
    codegen.emit(f);
    Function::pop(&f);
    LUMINOUS_INFO("Done.");
}

void test() {
    int v1{0};
    float v2{static_cast<float>(0.0f / +0.0f)};
    if (((v1 > 0) && (v2 < 0))) {
        v2 = 0;
    }
}

int main() {

    fake_compile_kernel("test", [](){
        Var<int> a = 0;
        Var<float> b = 0.f / 0.f;
        If(a > 0 && b < 0) {
            b = 0;
        };
    });

    float v2{0x1.99999ap-4f};
    cout << v2;

    return 0;
}
