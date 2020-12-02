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
    SET_LOG_LEVEL(warn)
    Context context{argc, argv};
    auto device = Device::create(&context);
    auto t = nloJson ();
    t["type"] = "translation";
    t["param"] = nloJson::array({1,2,3});

    auto r = nloJson ();
    r["type"] = "rotate";
    r["param"] = nloJson::array({1,0,0, 60});

    auto s = nloJson ();
    s["type"] = "scale";
    s["param"] = nloJson::array({2,2.7,2});

    auto ps = nloJson::array();
    ps.push_back(t);
    ps.push_back(r);
    ps.push_back(s);

    auto param = ParamSet(ps);


    auto trs = Plugin::create<Transform>(device.get(), "trs", param);

//    auto tex = Plugin::create<luminous::render::Texture<luminous::float3>>(device.get(), "constant_float3", param);

    cout << luminous::scaling(trs->scaling()).to_string() << endl;
    auto q = trs->rotation();
    cout << luminous::quaternion_to_matrix(q).to_string();
}