//
// Created by Zero on 2020/9/6.
//

#pragma once

#include <array>
#include <atomic>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "math/data_types.h"
#include "core/CObject.h"

namespace luminous::compute::dsl {

enum TypeCatalog : uint32_t{

    UNKNOWN,

    AUTO,

    BOOL,

    FLOAT,
    INT8,
    UINT8,
    INT16,
    UINT16,
    INT32,
    UINT32,
    INT64,
    UINT64,

    VECTOR2,
    VECTOR3,
    VECTOR4,
    VECTOR3_PACKED,

    MATRIX3,
    MATRIX4,

    ARRAY,
    CONSTANT,
    POINTER,
    REFERENCE,

    TEXTURE,

    ATOMIC,

    STRUCTURE
};

struct TypeDesc : Noncopyable {

public:
    TypeCatalog type{TypeCatalog::UNKNOWN};
    uint32_t size{0u};

    // for const, array, pointer and reference
    const TypeDesc *element_type{nullptr};
    uint32_t element_count{0u};

    // for textures
//    TextureAccess access{TextureAccess::READ_WRITE};

    // for structure
    std::vector<std::string> member_names;
    std::vector<const TypeDesc *> member_types;
    uint32_t alignment{0u};

    // uid
    [[nodiscard]] uint32_t uid() const noexcept {
        return _uid;
    };

private:
    uint32_t _uid{_uid_counter++};
    inline static uint32_t _uid_counter{1u};
};

struct AutoType {

    [[nodiscard]] static TypeDesc *desc() noexcept {
        static TypeDesc d;
        static std::once_flag flag;
        std::call_once(flag, [] {
            d.type = TypeCatalog::AUTO;
        });
        return &d;
    }

    // For initialization type checking in Function::var<Auto>
    template<typename... Args>
    explicit AutoType(Args &&...) {

    }
};

template<typename T>
struct Scalar {
    static_assert(std::is_arithmetic_v<T>);
    [[nodiscard]] static TypeDesc *desc() noexcept {
        static TypeDesc td;
        static std::once_flag flag;
        std::call_once(flag, [] {
            td.size = static_cast<uint32_t>(sizeof(T));
            if constexpr (std::is_same_v<bool, T>) {
                td.type = TypeCatalog::BOOL;
            } else if constexpr (std::is_same_v<float, T>) {
                td.type = TypeCatalog::FLOAT;
            } else if constexpr (std::is_same_v<int8_t, T>) {
                td.type = TypeCatalog::INT8;
            } else if constexpr (std::is_same_v<uint8_t, T>) {
                td.type = TypeCatalog::UINT8;
            } else if constexpr (std::is_same_v<int16_t, T>) {
                td.type = TypeCatalog::INT16;
            } else if constexpr (std::is_same_v<uint16_t, T>) {
                td.type = TypeCatalog::UINT16;
            } else if constexpr (std::is_same_v<int32_t, T>) {
                td.type = TypeCatalog::INT32;
            } else if constexpr (std::is_same_v<uint32_t, T>) {
                td.type = TypeCatalog::UINT32;
            } else if constexpr (std::is_same_v<int64_t, T>) {
                td.type = TypeCatalog::INT64;
            } else if constexpr (std::is_same_v<uint64_t, T>) {
                td.type = TypeCatalog::UINT64;
            }
        });
        return &td;
    }
};

template<typename T>
struct Constant {
    [[nodiscard]] static TypeDesc *desc() noexcept {
        static TypeDesc d;
        static std::once_flag flag;
        std::call_once(flag, [] {
            d.type = TypeCatalog::CONSTANT;
            d.element_type = T::desc();
        });
        return &d;
    }
};

template<typename T>
struct Atomic {
    [[nodiscard]] static TypeDesc *desc() noexcept {
        static TypeDesc d;
        static std::once_flag flag;
        std::call_once(flag, [] {
            d.type = TypeCatalog::ATOMIC;
            d.element_type = T::desc();
        });
        return &d;
    }
};

struct Matrix3 {
    [[nodiscard]] static TypeDesc *desc() noexcept {
        static TypeDesc d;
        static std::once_flag flag;
        std::call_once(flag, [] { d.type = TypeCatalog::MATRIX3; });
        return &d;
    }
};

struct Matrix4 {
    [[nodiscard]] static TypeDesc *desc() noexcept {
        static TypeDesc d;
        static std::once_flag flag;
        std::call_once(flag, [] { d.type = TypeCatalog::MATRIX4; });
        return &d;
    }
};

#define MAKE_VECTOR_TYPE_DESC(n, postfix)                \
    template<typename T>                                 \
    struct Vector##n##postfix {                          \
        static_assert(std::is_arithmetic_v<T>);          \
        [[nodiscard]] static TypeDesc *desc() noexcept { \
            static TypeDesc d;                           \
            static std::once_flag flag;                  \
            std::call_once(flag, [] {                    \
                d.type = TypeCatalog::VECTOR##n;         \
                d.element_type = Scalar<T>::desc();      \
            });                                          \
            return &d;                                   \
        }                                                \
    };

MAKE_VECTOR_TYPE_DESC(2, )
MAKE_VECTOR_TYPE_DESC(3, )
MAKE_VECTOR_TYPE_DESC(4, )
MAKE_VECTOR_TYPE_DESC(3, _packed)

#undef MAKE_VECTOR_TYPE_DESC

template<typename T, size_t N>
struct Array {
    [[nodiscard]] static TypeDesc *desc() noexcept {
        static TypeDesc d;
        static std::once_flag flag;
        std::call_once(flag, [] {
            d.type = TypeCatalog::ARRAY;
            d.element_type = T::desc();
            d.element_count = static_cast<uint32_t>(N);
        });
        return &d;
    }
};

template<typename T>
struct Pointer {
    [[nodiscard]] static TypeDesc *desc() noexcept {
        static TypeDesc d;
        static std::once_flag flag;
        std::call_once(flag, [] {
            d.type = TypeCatalog::POINTER;
            d.element_type = T::desc();
        });
        return &d;
    }
};

template<typename T>
struct Reference {
    [[nodiscard]] static TypeDesc *desc() noexcept {
        static TypeDesc d;
        std::once_flag flag;
        std::call_once(flag, [] {
            d.type = TypeCatalog::REFERENCE;
            d.element_type = T::desc();
        });
        return &d;
    }
};

template<typename T>
struct Structure {

    template<typename>
    static constexpr bool ALWAYS_FALSE = false;

    static_assert(ALWAYS_FALSE<T>, "Unregistered structure");
};

}