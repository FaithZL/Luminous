//
// Created by Zero on 2020/9/3.
//

#pragma once

#include "scalar_types.h"
#include <type_traits>
#include "util/header.h"

LUMINOUS_BEGIN

namespace vector {

namespace detail {
    template<typename T, uint32_t N, bool is_packed>
    constexpr auto vector_alignment = is_packed ? sizeof(T) : (sizeof(T) * (N + (N & 1u)));

    template<typename T, uint32_t>
    struct VectorStorage {};

    template<typename T>
    struct VectorStorage<T, 2> {
        T x, y;
        constexpr VectorStorage() noexcept : x{}, y{} {}
        explicit constexpr VectorStorage(T s) noexcept : x{s}, y{s} {}
        constexpr VectorStorage(T x, T y) noexcept : x{x}, y{y} {}
    };

    template<typename T>
    struct VectorStorage<T, 3> {
        T x, y, z;
        constexpr VectorStorage() noexcept : x{}, y{}, z{} {}
        explicit constexpr VectorStorage(T s) noexcept : x{s}, y{s}, z{s} {}
        constexpr VectorStorage(T x, T y, T z) noexcept : x{x}, y{y}, z{z} {}
    };

    template<typename T>
    struct VectorStorage<T, 4> {
        T x, y, z, w;
        constexpr VectorStorage() noexcept : x{}, y{}, z{}, w{} {}
        explicit constexpr VectorStorage(T s) noexcept : x{s}, y{s}, z{s}, w{s} {}
        constexpr VectorStorage(T x, T y, T z, T w) noexcept : x{x}, y{y}, z{z}, w{w} {}
    };
} // detail

    template<typename T, uint32_t N, bool is_packed>
    struct alignas(detail::vector_alignment<T, N, is_packed>) Vector : detail::VectorStorage<T, N> {

        using Storage = detail::VectorStorage<T, N>;

        constexpr Vector() noexcept : detail::VectorStorage<T, N>{static_cast<T>(0)} {}

        template<typename U>
        explicit constexpr Vector(U u) noexcept : detail::VectorStorage<T, N>{static_cast<T>(u)} {}

        template<typename... U,
                std::enable_if_t<sizeof...(U) == N, int> = 0>
        explicit constexpr Vector(U... u) noexcept : detail::VectorStorage<T, N>{static_cast<T>(u)...} {}

        template<typename Index>
        [[nodiscard]] T &operator[](Index i) noexcept { return reinterpret_cast<T(&)[N]>(*this)[i]; }

        template<typename Index>
        [[nodiscard]] T operator[](Index i) const noexcept { return reinterpret_cast<const T(&)[N]>(*this)[i]; }

    #define MAKE_ASSIGN_OP(op)                               \
    template<bool packed>                                    \
    Vector &operator op(Vector<T, N, packed> rhs) noexcept { \
        static_assert(N == 2 || N == 3 || N == 4);           \
        if constexpr (N == 2) {                              \
            Storage::x op rhs.x;                             \
            Storage::y op rhs.y;                             \
        } else if constexpr (N == 3) {                       \
            Storage::x op rhs.x;                             \
            Storage::y op rhs.y;                             \
            Storage::z op rhs.z;                             \
        } else {                                             \
            Storage::x op rhs.x;                             \
            Storage::y op rhs.y;                             \
            Storage::z op rhs.z;                             \
            Storage::w op rhs.w;                             \
        }                                                    \
        return *this;                                        \
    }                                                        \
    Vector &operator op(T rhs) noexcept {                    \
        static_assert(N == 2 || N == 3 || N == 4);           \
        if constexpr (N == 2) {                              \
            Storage::x op rhs;                               \
            Storage::y op rhs;                               \
        } else if constexpr (N == 3) {                       \
            Storage::x op rhs;                               \
            Storage::y op rhs;                               \
            Storage::z op rhs;                               \
        } else {                                             \
            Storage::x op rhs;                               \
            Storage::y op rhs;                               \
            Storage::z op rhs;                               \
            Storage::w op rhs;                               \
        }                                                    \
        return *this;                                        \
    }

        MAKE_ASSIGN_OP(+=)
        MAKE_ASSIGN_OP(-=)
        MAKE_ASSIGN_OP(*=)
        MAKE_ASSIGN_OP(/=)
        MAKE_ASSIGN_OP(%=)

    #undef MAKE_ASSIGN_OP
    };

}

LUMINOUS_END