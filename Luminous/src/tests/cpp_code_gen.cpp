//
// Created by Zero on 2020/9/20.
//

//#include <compute/dsl.h>
#include <compute/dsl_syntax.h>
#include <compute/codegen.h>
#include <compute/buffer.h>
#include "render/include/sampling.h"
#include "compute/device.h"
#include "compute/function.h"
//#include "compute/ray.h"
#include "core/math/data_types.h"


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

inline Expr<luminous::float3> cosine_sample_hemisphere(Expr<luminous::float2> u) {
    Var r = sqrt(u.x);
    Var phi = static_cast<float>(2.0f * M_PI) * u.y;
    return make_float3(r * cos(phi), r * sin(phi), sqrt(1.0f - u.x));
}


struct alignas(16) Struct_14 {
    float origin_x;
    float origin_y;
    float origin_z;
    float min_distance;
    float direction_x;
    float direction_y;
    float direction_z;
    float max_distance;
};

void test() {
    Struct_14 v1{};
    (v1.origin_x) = 0x0p+0f;
    auto v2{(v1.origin_x)};
    int v3{0};
    if (((v2 > 0) && (v3 < 0))) {
        v3 = 0;
    } else if ((v3 > 0)) {
        v2 = 0;
    } else {
        v2 = 0;
    }
    int v4{-1};
    while (true) {
        int v5{v4};
        if (!(v5 < 0)) {
            break;
        }
        v4 += 1;
        v4 += 2;
    }
    float2 v6 = luminous::make_float2(0);

    v6[0] = 0x1.68p+6f;
    float v7{sqrt((v6.x))};
    float v8{(0x1.921fb6p+2f * (v6.y))};
    float3 v9{luminous::make_float3((v7 * cos(v8)), (v7 * sin(v8)), sqrt((0x1p+0f - (v6.x))))};
    v9[2] = 0x1.4p+6f;
}

int main() {

    fake_compile_kernel("test", [](){

        Var a = luminous::render::sampling::cosine_sample_hemisphere(make_float2(0,0));
//        Var<Ray> ray;
//        ray.origin_x = 0;

//        Var<int> a = ray.origin_x;
//        Var b = 0;
//        If(a > 0 && b < 0) {
//            b = 0;
//        } Elif (b > 0) {
//            a = 0;
//        } Else {
//            a = 0;
//        };
//
//
//        auto foo = [&](Expr<int> x) {
//            Var xx = x;
//            return xx < 0;
//        };
//        Var x = -1;
//        While (foo(x)) {
//            x += 1;
//            x += 2;
//        };
//
//        Var<luminous::float2> c = make_float2(0);
//
//        c[0] = 90;
//
//        Var aa = cosine_sample_hemisphere(c);
//        aa[3] = 80;

    });



    float v2{0x1.99999ap-4f};
    cout << v2;

    return 0;
}
