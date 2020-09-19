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


    class TextureView {

    private:
        std::shared_ptr<Texture> _texture{nullptr};

    public:
        TextureView() noexcept = default;
        explicit TextureView(std::shared_ptr<Texture> texture) noexcept: _texture{std::move(texture)} {}
        [[nodiscard]] Texture *texture() const noexcept { return _texture.get(); }

        [[nodiscard]] auto copy_from(const void *data) { return [texture = _texture, data](Dispatcher &d) { texture->copy_from(d, data); }; }
        [[nodiscard]] auto copy_to(void *data) const { return [texture = _texture, data](Dispatcher &d) { texture->copy_to(d, data); }; }

        template<typename T>
        [[nodiscard]] auto copy_from(const BufferView<T> &bv) {
            LUISA_WARNING_IF_NOT(pixel_format<T> == _texture->format(), "Texture pixel format and buffer type mismatch.");
            return [texture = _texture, buffer = bv.buffer()->shared_from_this(), offset = bv.byte_offset()](Dispatcher &d) {
                texture->copy_from(d, buffer.get(), offset);
            };
        }

        template<typename T>
        [[nodiscard]] auto copy_to(BufferView<T> &bv) const {
            LUISA_WARNING_IF_NOT(pixel_format<T> == _texture->format(), "Texture pixel format and buffer type mismatch.");
            return [texture = _texture, buffer = bv.buffer()->shared_from_this(), offset = bv.byte_offset()](Dispatcher &d) {
                texture->copy_to(d, buffer.get(), offset);
            };
        }

        [[nodiscard]] auto copy_to(TextureView &tv) const {
            return [texture = _texture, other = tv.texture()->shared_from_this()](Dispatcher &d) {
                texture->copy_to(d, other.get());
            };
        }

        [[nodiscard]] uint32_t width() const noexcept { return _texture->width(); }
        [[nodiscard]] uint32_t height() const noexcept { return _texture->height(); }
        [[nodiscard]] PixelFormat format() const noexcept { return _texture->format(); }

        void clear_cache() { _texture->clear_cache(); }

        [[nodiscard]] uint32_t channels() const noexcept { return _texture->channels(); }
        [[nodiscard]] uint32_t pixel_byte_size() const noexcept { return _texture->pixel_byte_size(); }
        [[nodiscard]] uint32_t pitch_byte_size() const noexcept { return _texture->pitch_byte_size(); }
        [[nodiscard]] uint32_t byte_size() const noexcept { return _texture->byte_size(); }
        [[nodiscard]] uint32_t pixel_count() const noexcept { return _texture->pixel_count(); }

        // For DSL
        template<typename UV>
        [[nodiscard]] auto read(UV &&uv) const noexcept {
            using namespace luminous::compute::dsl;
            Expr uv_expr{std::forward<UV>(uv)};
            auto tex = Variable::make_texture_argument(_texture);
            Function::current().mark_texture_read(_texture.get());
            return Expr<float4>{Variable::make_temporary(nullptr, std::make_unique<TextureExpr>(TextureOp::READ, tex, uv_expr.variable()))};
        }

        template<typename UV>
        [[nodiscard]] auto sample(UV &&uv) const noexcept {
            using namespace luminous::compute::dsl;
            Expr uv_expr{std::forward<UV>(uv)};
            auto tex = Variable::make_texture_argument(_texture);
            Function::current().mark_texture_sample(_texture.get());
            return Expr<float4>{Variable::make_temporary(nullptr, std::make_unique<TextureExpr>(TextureOp::SAMPLE, tex, uv_expr.variable()))};
        }

        [[nodiscard]] auto save(std::filesystem::path path) const {
            return [texture = _texture, path = std::move(path)](Dispatcher &d) { texture->save(d, path); };
        }

        template<typename UV, typename Value>
        void write(UV &&uv, Value &&value) noexcept {
            using namespace luminous::compute::dsl;
            Expr uv_expr{std::forward<UV>(uv)};
            Expr value_expr{std::forward<Value>(value)};
            auto tex = Variable::make_texture_argument(_texture);
            Function::current().mark_texture_write(_texture.get());
            Function::current().add_statement(std::make_unique<ExprStmt>(std::make_unique<TextureExpr>(TextureOp::WRITE, tex, uv_expr.variable(), value_expr.variable())));
        }

        [[nodiscard]] bool empty() const noexcept { return _texture == nullptr; }
        [[nodiscard]] bool is_hdr() const noexcept { return _texture->is_hdr(); }
    };

    [[nodiscard]] inline TextureView Texture::view() noexcept {
        return TextureView{shared_from_this()};
    }


}