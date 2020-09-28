//
// Created by Zero on 2020/9/20.
//

//#include <compute/dsl.h>
#include <compute/dsl_syntax.h>
#include <compute/codegen.h>
#include <compute/buffer.h>
#include "compute/device.h"
#include "compute/function.h"
//#include "compute/ray.h"
#include "core/math/data_types.h"


using namespace std;

using namespace luminous;

using namespace luminous::compute;

using namespace luminous::compute::dsl;

void test() {
    int v1{0};
    float v2{static_cast<float>(0.0f / +0.0f)};
    if (((v1 > 0) && (v2 < 0))) {
        v2 = 0;
    } else if ((v2 > 0)) {
        v1 = 0;
    } else {
        v1 = 0;
    }
    while (v1) {
        v1 += 1;
    }
    float2 v3{luminous::make_float2(0)};
    float v4{sqrt((v3.x))};
    float v5{(0x1.921fb6p+2f * (v3.y))};
    float3 v6{make_float3((v4 * cos(v5)), (v4 * sin(v5)), sqrt((0x1p+0f - (v3.x))))};
    switch (v1) {
        case 1: {
            v1 = 0;
            break;
        }
        case 2: {
            v2 = 0;
            break;
        }
        default: {
            break;
        }
    }
    int v7{5};
    do {
        v7 = 5;
        break;
    } while ((v1 < 10));
}

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

inline Expr<float3> cosine_sample_hemisphere(Expr<float2> u) {
    Var r = sqrt(u.x());
    Var phi = static_cast<float>(2.0f * M_PI) * u.y();
    return make_float3(r * cos(phi), r * sin(phi), sqrt(1.0f - u.x()));
}

int main() {

    fake_compile_kernel("test", [](){
        Var<int> a = 1;
        Var b = 0;
        If(a > 0 && b < 0) {
            b = 0;
        } Elif (b > 0) {
            a = 0;
        } Else {
            a = 0;
        };


        auto foo = [&](Expr<int> x) {
            Var xx = x;
            return xx < 0;
        };
        Var x = -1;
        While (foo(x)) {
            x += 1;
            x += 2;
        };
        While(a) {
            a += 1;
        };

        Var<float2> c = make_float2(0);

        Var aa = cosine_sample_hemisphere(c);

        Switch(a) {
            Case(1) {
                a = 0;
            };
            Case(2) {
                b = 0;
            };
            Default {

            };
        };
        Var i = 5;

    });



    float v2{0x1.99999ap-4f};
    cout << v2;

    return 0;
}
