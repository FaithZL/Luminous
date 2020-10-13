//
// Created by Zero on 2020/10/13.
//

#pragma once

#include <fstream>
#include <core/header.h>
#include <core/string_util.h>

namespace luminous {
    inline namespace utility {

        inline nloJson create_json_from_file(const std::string &fn) {
            std::ifstream fst;
            fst.open(fn.c_str());
            std::stringstream buffer;
            buffer << fst.rdbuf();
            std::string str = buffer.str();
            str = jsonc_to_json(str);
            return nloJson::parse(str);
        }

        class SceneParser {
            void start();

            void parse(const nloJson &);

            void end();
        };

    }
}