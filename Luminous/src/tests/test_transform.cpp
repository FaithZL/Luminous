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
    cout << setw(4) << param.json();

    auto trs = Plugin::create<Transform>(device.get(), "trs", param);

}