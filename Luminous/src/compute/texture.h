//
// Created by Zero on 2020/9/6.
//

#pragma once

#include <cstdint>
#include <math/data_types.h>
#include <util/logging.h>
#include <compute/buffer.h>

namespace luminous::compute {


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

class TextureView;

class Texture : public Noncopyable, public std::enable_shared_from_this<Texture> {

protected:
    uint32_t _width;
    uint32_t _height;
    PixelFormat _format;

public:
    Texture(uint32_t width, uint32_t height, PixelFormat format) noexcept
            : _width{width}, _height{height}, _format{format} {}
    virtual ~Texture() noexcept = default;
    [[nodiscard]] uint32_t width() const noexcept { return _width; }
    [[nodiscard]] uint32_t height() const noexcept { return _height; }
    [[nodiscard]] PixelFormat format() const noexcept { return _format; }
    virtual void clear_cache() = 0;

    virtual void copy_from(Dispatcher &dispatcher, Buffer *buffer, size_t offset) = 0;
    virtual void copy_to(Dispatcher &dispatcher, Buffer *buffer, size_t offset) = 0;
    virtual void copy_to(Dispatcher &dispatcher, Texture *texture) = 0;
    virtual void copy_from(Dispatcher &dispatcher, const void *data) = 0;
    virtual void copy_to(Dispatcher &dispatcher, void *data) = 0;

    void save(Dispatcher &dispatch, const std::filesystem::path &path);

    [[nodiscard]] TextureView view() noexcept;

    [[nodiscard]] uint32_t channels() const noexcept {
        if (_format == PixelFormat::R8U || _format == PixelFormat::R32F) { return 1u; }
        if (_format == PixelFormat::RG8U || _format == PixelFormat::RG32F) { return 2u; }
        return 4u;
    }

    [[nodiscard]] bool is_hdr() const noexcept {
        return _format == PixelFormat::R32F || _format == PixelFormat::RG32F || _format == PixelFormat::RGBA32F;
    }

    [[nodiscard]] uint32_t pixel_byte_size() const noexcept {
        if (_format == PixelFormat::R8U ||
            _format == PixelFormat::RG8U ||
            _format == PixelFormat::RGBA8U) {
            return channels();
        }
        return sizeof(float) * channels();
    }

    [[nodiscard]] uint32_t pitch_byte_size() const noexcept { return pixel_byte_size() * width(); }
    [[nodiscard]] uint32_t byte_size() const noexcept { return pitch_byte_size() * height(); }
    [[nodiscard]] uint32_t pixel_count() const noexcept { return width() * height(); }
};



}