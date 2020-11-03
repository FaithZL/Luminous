//
// Created by Zero on 2020/10/13.
//

#pragma once

#include <fstream>
#include "param_set.h"
#include <core/string_util.h>

namespace luminous {
    inline namespace utility {
        using luminous::utility::ParamSet;
        inline ParamSet create_json_from_file(const std::string &fn) {
            std::ifstream fst;
            fst.open(fn.c_str());
            std::stringstream buffer;
            buffer << fst.rdbuf();
            std::string str = buffer.str();
            str = jsonc_to_json(str);
            return ParamSet(nloJson::parse(str));
        }

        class SceneParser {
            void start();

            void parse(const nloJson &);

            void end();
        };

    }
}