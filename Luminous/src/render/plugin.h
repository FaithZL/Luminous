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
#include "util/param_set.h"

#include <core/platform.h>
#include <core/concepts.h>
#include <core/logging.h>
#include <core/math/data_types.h>
#include <compute/device.h>
#include "util/param_set.h"

namespace luminous::render {

#define LUMINOUS_MAKE_PLUGIN_BASE_CLASS_MATCHER_AND_NAME(BaseClass)                       \
    class BaseClass;                                                                      \
    namespace detail {                                                                    \
        template<typename T, std::enable_if_t<std::is_base_of_v<BaseClass, T>, int> = 0>  \
        auto plugin_base_class_match(T *) { return static_cast<BaseClass *>(nullptr); }   \
                                                                                          \
        template<typename T, std::enable_if_t<std::is_same_v<BaseClass, T>, int> = 0>     \
        constexpr std::string_view plugin_base_class_name(T *) {                          \
            using namespace std::string_view_literals;                                    \
            return #BaseClass""sv;                                                        \
        }                                                                                 \
    }

#define LUMINOUS_MAKE_PLUGIN_BASE_CLASS_MATCHER_AND_NAME_T(BaseClass,U)                                 \
    template<typename N>                                                                                \
    class BaseClass;                                                                                    \
    namespace detail {                                                                                  \
        template<typename T, std::enable_if_t<std::is_base_of_v<BaseClass<U>, T>, int> = 0>             \
        auto plugin_base_class_match(T *) { return static_cast<BaseClass<U> *>(nullptr); }              \
        template<typename T, std::enable_if_t<std::is_same_v<BaseClass<U>, T>, int> = 0>                \
        constexpr std::string_view plugin_base_class_name(T *) {                                        \
            using namespace std::string_view_literals;                                                  \
            return #BaseClass""sv;                                                                      \
        }                                                                                               \
    }

LUMINOUS_MAKE_PLUGIN_BASE_CLASS_MATCHER_AND_NAME(Filter)
LUMINOUS_MAKE_PLUGIN_BASE_CLASS_MATCHER_AND_NAME(Film)
LUMINOUS_MAKE_PLUGIN_BASE_CLASS_MATCHER_AND_NAME(Camera)
LUMINOUS_MAKE_PLUGIN_BASE_CLASS_MATCHER_AND_NAME(Shape)
LUMINOUS_MAKE_PLUGIN_BASE_CLASS_MATCHER_AND_NAME(Transform)
LUMINOUS_MAKE_PLUGIN_BASE_CLASS_MATCHER_AND_NAME(Material)
LUMINOUS_MAKE_PLUGIN_BASE_CLASS_MATCHER_AND_NAME(Integrator)
LUMINOUS_MAKE_PLUGIN_BASE_CLASS_MATCHER_AND_NAME(Sampler)
LUMINOUS_MAKE_PLUGIN_BASE_CLASS_MATCHER_AND_NAME(Light)

LUMINOUS_MAKE_PLUGIN_BASE_CLASS_MATCHER_AND_NAME_T(Texture,float)
LUMINOUS_MAKE_PLUGIN_BASE_CLASS_MATCHER_AND_NAME_T(Texture,float3)
LUMINOUS_MAKE_PLUGIN_BASE_CLASS_MATCHER_AND_NAME_T(Texture,float4)

#undef LUMINOUS_MAKE_PLUGIN_BASE_CLASS_MATCHER_AND_NAME
#undef LUMINOUS_MAKE_PLUGIN_BASE_CLASS_MATCHER_AND_NAME_T

    using luminous::compute::Device;
    using namespace luminous;
    class Plugin : public Noncopyable {
    private:
        Device *_device{nullptr};

    public:
        Plugin(Device *device, const ParamSet &) noexcept
        : _device{device} {

        }

        virtual ~Plugin() noexcept = default;

        [[nodiscard]] Device *device() const noexcept {
            return _device;
        }

        template<typename T>
        using PluginBaseClass = std::remove_pointer_t<decltype(detail::plugin_base_class_match(static_cast<T *>(nullptr)))>;

        template<typename T>
        static constexpr auto plugin_base_class_name() noexcept {
            return detail::plugin_base_class_name(static_cast<PluginBaseClass<T> *>(nullptr));
        }


        template<typename T>
        [[nodiscard]] static std::unique_ptr<T> create(
                Device *device,
                std::string_view derived_name_pascal_case,
                const ParamSet &params) {

            auto base_name = pascal_to_snake_case(plugin_base_class_name<T>());
            auto derived_name = pascal_to_snake_case(derived_name_pascal_case);
            auto plugin_dir = device->context().runtime_path("bin") / "plugins";
            using PluginCreator = T *(Device *, const ParamSet &);
            auto moduleName = serialize("luminous-", base_name, "-", derived_name);
            auto creator = device->context().load_dynamic_function<PluginCreator>(plugin_dir,
                                                                                  moduleName,
                                                                                  "create");
            return std::unique_ptr<T>{creator(device, params)};
        }
    };

}

#define LUMINOUS_EXPORT_PLUGIN_CREATOR(PluginClass)                                                                                                    \
    extern "C" LUMINOUS_EXPORT ::luminous::render::Plugin *create(::luminous::compute::Device *device,                                                 \
                                                            const luminous::utility::ParamSet &params) {                                                \
        LUMINOUS_INFO("Creating instance of class ", #PluginClass, ", category: ", ::luminous::render::Plugin::plugin_base_class_name<PluginClass>()); \
        return new PluginClass{device, params};                                                                                                        \
    }
