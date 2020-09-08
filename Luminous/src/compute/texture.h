//
// Created by Zero on 2020/9/6.
//

#pragma once

#include <cstdint>
#include <math/data_types.h>
#include <compute/buffer.h>

namespace luminous::compute {

enum TextureAccess : uint32_t {
    READ,
    WRITE,
    READ_WRITE,
    SAMPLE
};

enum PixelFormat : uint32_t {
    R8U, RG8U, RGBA8U,
    R32F, RG32F, RGBA32F,
};

namespace detail {

template<typename T>
struct PixelFormatImpl {

    template<typename U>
    static constexpr auto always_false = false;

    static_assert(always_false<T>, "Unsupported type for pixel format.");
};

#define MAKE_PIXEL_FORMAT_OF_TYPE(Type, f)          \
template<>                                          \
struct PixelFormatImpl<Type> {                      \
    static constexpr auto format = PixelFormat::f;  \
};                                                  \

MAKE_PIXEL_FORMAT_OF_TYPE(uchar, R8U)
MAKE_PIXEL_FORMAT_OF_TYPE(uchar2, RG8U)
MAKE_PIXEL_FORMAT_OF_TYPE(uchar4, RGBA8U)
MAKE_PIXEL_FORMAT_OF_TYPE(float, R32F)
MAKE_PIXEL_FORMAT_OF_TYPE(float2, RG32F)
MAKE_PIXEL_FORMAT_OF_TYPE(float4, RGBA32F)

#undef MAKE_PIXEL_FORMAT_OF_TYPE

}

template<typename T>
constexpr auto pixel_format = detail::PixelFormatImpl<T>::format;

class Texture : public std::enable_shared_from_this<Texture> {

protected:
    uint32_t _width;
    uint32_t _height;
    PixelFormat _format;

    virtual void _copy_from(Dispatcher &dispatcher, Buffer *buffer, size_t offset) = 0;
    virtual void _copy_to(Dispatcher &dispatcher, Buffer *buffer, size_t offset) = 0;
    virtual void _copy_to(Dispatcher &dispatcher, Texture *texture) = 0;
    virtual void _copy_from(Dispatcher &dispatcher, const void *data) = 0;
    virtual void _copy_to(Dispatcher &dispatcher, void *data) = 0;

public:
    Texture(uint32_t width, uint32_t height, PixelFormat format) noexcept
            : _width{width},
            _height{height},
            _format{format} {

    }

    virtual ~Texture() noexcept = default;

    [[nodiscard]] uint32_t width() const noexcept { return _width; }

    [[nodiscard]] uint32_t height() const noexcept { return _height; }

    [[nodiscard]] PixelFormat format() const noexcept { return _format; }

    virtual void clear_cache() = 0;

    [[nodiscard]] auto copy_from(const void *data) {
        auto ret = [this, data](Dispatcher &d) {
            _copy_from(d, data);
        };
        return ret;
    }
    [[nodiscard]] auto copy_to(void *data) {
        auto ret = [this, data](Dispatcher &d) {
            _copy_to(d, data);
        };
        return ret;
    }

    template<typename T>
    [[nodiscard]] auto copy_from(const BufferView<T> &buffer) {
        LUMINOUS_WARNING_IF_NOT(pixel_format<T> == _format, "Texture pixel format and buffer type mismatch.");
        return [this, buffer](Dispatcher &d) {
            _copy_from(d, buffer.buffer(), buffer.byte_offset());
        };
    }

    template<typename T>
    [[nodiscard]] auto copy_to(const BufferView<T> &buffer) {
        LUMINOUS_WARNING_IF_NOT(pixel_format<T> == _format, "Texture pixel format and buffer type mismatch.");
        return [this, buffer](Dispatcher &d) {
            _copy_to(d, buffer.buffer(), buffer.byte_offset());
        };
    }

    [[nodiscard]] auto copy_to(Texture &texture) {
        return [this, &texture](Dispatcher &d) {
            _copy_to(d, &texture);
        };
    }

    [[nodiscard]] uint32_t channels() const noexcept {
        if (_format == PixelFormat::R8U || _format == PixelFormat::R32F) {
            return 1u;
        }
        if (_format == PixelFormat::RG8U || _format == PixelFormat::RG32F) {
            return 2u;
        }
        return 4u;
    }

    [[nodiscard]] uint32_t pixel_byte_size() const noexcept {
        if (_format == PixelFormat::R8U || _format == PixelFormat::RG8U || _format == PixelFormat::RGBA8U) {
            return channels();
        }
        return sizeof(float) * channels();
    }

    [[nodiscard]] uint32_t pitch_byte_size() const noexcept {
        return pixel_byte_size() * _width;
    }

    [[nodiscard]] uint32_t byte_size() const noexcept {
        return pitch_byte_size() * _height;
    }

    [[nodiscard]] uint32_t pixel_count() const noexcept {
        return _width * _height;
    }
};

}