//
// Created by Zero on 2020/10/31.
//

#include "render/include/transform.h"
#include "iomanip"

using namespace std;
using namespace luminous::compute;
using namespace luminous::compute::dsl;
using namespace luminous;
using luminous::render::ParamSet;
using luminous::render::Plugin;
using luminous::render::Transform;

int main(int argc, char *argv[]) {
    SET_LOG_LEVEL(debug)
    Context context{argc, argv};
    auto device = Device::create(&context);
    auto ps = nloJson ();
    ps["type"] = "translation";
    ps["param"] = nloJson::array({1,2,3});
    auto param = ParamSet(ps);


    auto trs = Plugin::create<Transform>(device.get(), "trs", param);
    auto mat1 = trs->matrix(0);
//    cout << mat1.to_string();

//    ps["type"] = "rotate";
//    ps["param"] = nloJson::array({1,0,0, 30});
//    trs = Transform::create<Transform>(device.get(), "trs", ps);
//
//    cout << trs->matrix(0).to_string() << endl;
//    auto q = matrix_to_quaternion(trs->matrix(0));
//    cout << matrix_to_quaternion(trs->matrix(0)).to_string() << endl;
//    auto m = quaternion_to_matrix(q);
//    cout << m.to_string();

    float4 qn = luminous::make_float4(-1, 0, 0, 0.5);
    float4 qn2 = luminous::make_float4(1,0,0,-0.5);
    auto mat = quaternion_to_matrix(qn);
    auto mat2 = quaternion_to_matrix(qn2);
//    cout << mat.to_string() << endl << endl << mat2.to_string();
    cout << matrix_to_quaternion(mat).to_string() << endl;
    cout << matrix_to_quaternion(mat2).to_string();
//    cout << qn.to_string() << endl << matrix_to_quaternion(mat).to_string();
}