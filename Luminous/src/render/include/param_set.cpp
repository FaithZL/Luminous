//
// Created by Zero on 2020/11/14.
//


#include "param_set.h"
#include "render/plugin.h"

namespace luminous::render {
    using luminous::render::Plugin;
    template<typename BaseClass>
    [[nodiscard]] std::shared_ptr<BaseClass> ParamSet::parse() const {
        return Plugin::create<BaseClass>(_device, _json["subType"], *this);
    }
}