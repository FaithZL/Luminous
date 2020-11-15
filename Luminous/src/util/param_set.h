//
// Created by Zero on 2020/10/31.
//

#pragma once

#include <compute/dsl_syntax.h>
#include "core/header.h"
#include "compute/device.h"

namespace luminous {
    using luminous::compute::Device;
    inline namespace utility {
        using namespace std;
        class ParamSet {
        private:
            nloJson _json;
            string _key;
            const Device * _device;
        private:


#define LUMINOUS_MAKE_AS_TYPE_FUNC(type) [[nodiscard]] type _as_##type() const noexcept {   \
            return static_cast<type>(_json);                                                \
        }

#define LUMINOUS_MAKE_AS_TYPE_VEC2(type) [[nodiscard]] type##2 _as_##type##2() const noexcept { \
            return make_##type##2(this->at(0).as_##type(), this->at(1).as_##type());                                    \
        }                                                                                       \

#define LUMINOUS_MAKE_AS_TYPE_VEC3(type) [[nodiscard]] type##3 _as_##type##3() const noexcept { \
            return make_##type##3(this->at(0).as_##type(), this->at(1).as_##type(), this->at(2).as_##type());     \
        }
#define LUMINOUS_MAKE_AS_TYPE_VEC4(type) [[nodiscard]] type##4 _as_##type##4() const noexcept { \
            return make_##type##4(this->at(0).as_##type(), this->at(1).as_##type(), this->at(2).as_##type(), this->at(3).as_##type());          \
        }                                                                                       \
        template<typename T, std::enable_if_t<std::is_same_v<T, type##4>, int> = 0>            \
        T _as() const {                                                                     \
            return _as_##type##4();                                                            \
        }
#define LUMINOUS_MAKE_AS_TYPE_VEC(type)   \
        LUMINOUS_MAKE_AS_TYPE_VEC2(type)  \
        LUMINOUS_MAKE_AS_TYPE_VEC3(type)  \
        LUMINOUS_MAKE_AS_TYPE_VEC4(type)


#define LUMINOUS_MAKE_AS_TYPE_MAT(type)    \
        LUMINOUS_MAKE_AS_TYPE_MAT3X3(type) \
        LUMINOUS_MAKE_AS_TYPE_MAT4X4(type)


#define LUMINOUS_MAKE_AS_TYPE_MAT3X3(type) [[nodiscard]] float3x3 _as_##type##3x3() const noexcept { \
            return make_##type##3x3(                                                                 \
                    this->at(0)._as_##type##3(),                                                     \
                    this->at(1)._as_##type##3(),                                                     \
                    this->at(2)._as_##type##3());                                                    \
        }
#define LUMINOUS_MAKE_AS_TYPE_MAT4X4(type) [[nodiscard]] float4x4 _as_##type##4x4() const noexcept { \
            return make_##type##4x4(                                                                 \
                    this->at(0)._as_##type##4(),                                                     \
                    this->at(1)._as_##type##4(),                                                     \
                    this->at(2)._as_##type##4(),                                                     \
                    this->at(3)._as_##type##4());                                                    \
        }

            LUMINOUS_MAKE_AS_TYPE_FUNC(int)
            LUMINOUS_MAKE_AS_TYPE_FUNC(uint)
            LUMINOUS_MAKE_AS_TYPE_FUNC(float)
            LUMINOUS_MAKE_AS_TYPE_FUNC(string)

            LUMINOUS_MAKE_AS_TYPE_VEC(uint)
            LUMINOUS_MAKE_AS_TYPE_VEC(int)
            LUMINOUS_MAKE_AS_TYPE_VEC(float)

            LUMINOUS_MAKE_AS_TYPE_MAT(float)

#undef LUMINOUS_MAKE_AS_TYPE_FUNC

#undef LUMINOUS_MAKE_AS_TYPE_VEC
#undef LUMINOUS_MAKE_AS_TYPE_VEC2
#undef LUMINOUS_MAKE_AS_TYPE_VEC3
#undef LUMINOUS_MAKE_AS_TYPE_VEC4

#undef LUMINOUS_MAKE_AS_TYPE_MAT
#undef LUMINOUS_MAKE_AS_TYPE_MAT3X3
#undef LUMINOUS_MAKE_AS_TYPE_MAT4X4


        public:
            ParamSet() {}
            ParamSet(const nloJson &json,
                     const string &key = "",
                     const Device *device = nullptr):
                    _json(json),
                    _key(key),
                    _device(device) {

            }

            void setJson(const nloJson &json) { _json = json; }
            nloJson json() const { return _json; }

            [[nodiscard]] ParamSet get(const std::string &key) const {
                return ParamSet(_json[key], key, _device);
            }

            [[nodiscard]] ParamSet at(uint idx) const {
                return ParamSet(_json.at(idx), "", _device);
            }

#define LUMINOUS_MAKE_AS_TYPE_SCALAR(type) [[nodiscard]] type as_##type(type val = 0) const {                   \
            try {                                                                                               \
                return _as_##type();                                                                            \
            } catch (const std::runtime_error &e) {                                                             \
                LUMINOUS_WARNING("Error occurred while parsing parameter, using default value: \"", val, "\""); \
                return val;                                                                                     \
            }                                                                                                   \
        }                                                                                                       \
        template<typename T, std::enable_if_t<std::is_same_v<T, type>, int> = 0>           \
        T as() const {                                                                     \
            return as_##type();                                                            \
        }\

#define LUMINOUS_MAKE_AS_TYPE_VEC2(type) [[nodiscard]] type##2 as_##type##2(type##2 val = make_##type##2()) const {      \
            try {                                                                                                        \
                return _as_##type##2();                                                                                  \
            } catch (const std::runtime_error &e) {                                                                      \
                LUMINOUS_WARNING("Error occurred while parsing parameter, using default value: \"(", val.to_string() , ")\""); \
                return val;                                                                                              \
            }                                                                                                            \
        } \
        template<typename T, std::enable_if_t<std::is_same_v<T, type##2>, int> = 0>            \
        T as() const {                                                                     \
            return as_##type##2();                                                            \
        }
#define LUMINOUS_MAKE_AS_TYPE_VEC3(type) [[nodiscard]] type##3 as_##type##3(type##3 val = make_##type##3()) const {        \
            try {                                                                                                          \
                return _as_##type##3();                                                                                    \
            } catch (const std::runtime_error &e) {                                                                        \
                LUMINOUS_WARNING("Error occurred while parsing parameter, using default value: \"(", val.to_string() , ")\""); \
                return val;                                                                                              \
            }                                                                                                            \
        } \
        template<typename T, std::enable_if_t<std::is_same_v<T, type##3>, int> = 0>            \
        T as() const {                                                                     \
            return as_##type##3();                                                            \
        }
#define LUMINOUS_MAKE_AS_TYPE_VEC4(type) [[nodiscard]] type##4 as_##type##4(type##4 val = make_##type##4()) const {        \
            try {                                                                                                          \
                return _as_##type##4();                                                                                    \
            } catch (const std::runtime_error &e) {                                                                        \
                LUMINOUS_WARNING("Error occurred while parsing parameter, using default value: \"(", val.to_string() , ")\""); \
                return val;                                                                                              \
            }                                                                                                            \
        } \
        template<typename T, std::enable_if_t<std::is_same_v<T, type##4>, int> = 0>            \
        T as() const {                                                                     \
            return as_##type##4();                                                            \
        }
#define LUMINOUS_MAKE_AS_TYPE_MAT3X3(type) [[nodiscard]] type##3x3 as_##type##3x3(type##3x3 val = make_##type##3x3()) const noexcept { \
            try {                                                                                                                       \
                return _as_##type##3x3(); \
            } catch (const std::runtime_error &e) { \
                LUMINOUS_WARNING("Error occurred while parsing parameter, using default value: \"(", val.to_string() , ")\""); \
                return val; \
            } \
        }
#define LUMINOUS_MAKE_AS_TYPE_MAT4X4(type) [[nodiscard]] type##4x4 as_##type##4x4(type##4x4 val = make_##type##4x4()) const noexcept { \
            try {                                                                                                                       \
                return _as_##type##4x4(); \
            } catch (const std::runtime_error &e) { \
                LUMINOUS_WARNING("Error occurred while parsing parameter, using default value: \"(", val.to_string() , ")\""); \
                return val; \
            } \
        }

            template<typename BaseClass>
            [[nodiscard]] auto parse_or_null() const {
                std::shared_ptr<BaseClass> p = nullptr;
                try {
                    p = parse<BaseClass>();
                } catch (const std::runtime_error &e) {
                    LUMINOUS_WARNING("Error occurred while parsing parameter, returning null");
                }
                return p;
            }

            template<typename BaseClass>
            [[nodiscard]] std::shared_ptr<BaseClass> parse() const;

            LUMINOUS_MAKE_AS_TYPE_MAT3X3(float)
            LUMINOUS_MAKE_AS_TYPE_MAT4X4(float)

            LUMINOUS_MAKE_AS_TYPE_SCALAR(float)
            LUMINOUS_MAKE_AS_TYPE_SCALAR(uint)
            LUMINOUS_MAKE_AS_TYPE_SCALAR(int)

#define LUMINOUS_MAKE_AS_TYPE_VEC(type)  \
        LUMINOUS_MAKE_AS_TYPE_VEC2(type) \
        LUMINOUS_MAKE_AS_TYPE_VEC3(type) \
        LUMINOUS_MAKE_AS_TYPE_VEC4(type)

            LUMINOUS_MAKE_AS_TYPE_VEC(int)
            LUMINOUS_MAKE_AS_TYPE_VEC(uint)
            LUMINOUS_MAKE_AS_TYPE_VEC(float)


#undef LUMINOUS_MAKE_AS_TYPE_SCALAR
#undef LUMINOUS_MAKE_AS_TYPE_VEC
#undef LUMINOUS_MAKE_AS_TYPE_VEC2
#undef LUMINOUS_MAKE_AS_TYPE_VEC3
#undef LUMINOUS_MAKE_AS_TYPE_VEC4
#undef LUMINOUS_MAKE_AS_TYPE_MAT3X3
#undef LUMINOUS_MAKE_AS_TYPE_MAT4X4
        };
    }


}