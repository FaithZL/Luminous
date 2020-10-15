//
// Created by Zero on 2020/10/15.
//

#pragma once

#include <variant>
#include <string>
#include <unordered_map>
#include <type_traits>
#include <functional>
#include <memory>
#include <iostream>

#include <core/platform.h>
#include <core/concepts.h>
#include <core/logging.h>
#include <core/math/data_types.h>
#include <compute/device.h>

namespace luminous::render {
#define LUMINOUS_MAKE_PLUGIN_BASE_CLASS_MATCHER_AND_NAME(BaseClass)                       \
    class BaseClass;                                                                      \
    namespace detail {                                                                    \
        template<typename T, std::enable_if_t<std::is_base_of_v<BaseClass, T>, int> = 0>  \
        auto plugin_base_class_match(T *) { return static_cast<BaseClass *>(nullptr); }   \
                                                                                          \
        template<typename T, std::enable_if_t<std::is_same_v<BaseClass, T>, int> = 0>     \
        constexpr std::string_view plguin_base_class_name(T *) {                          \
            using namespace std::string_view_literals;                                    \
            return #BaseClass""sv;                                                        \
        }                                                                                 \
    }
}