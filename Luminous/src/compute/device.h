//
// Created by Zero on 2020/9/6.
//

#pragma once

#include <memory>

#include <compute/buffer.h>
#include <compute/function.h>
#include <compute/kernel.h>
#include <compute/texture.h>
#include <core/context.h>

namespace luminous::compute {
    class Device : Noncopyable {
    protected:
//        Context *_context{nullptr};

        [[nodiscard]] virtual std::shared_ptr<Buffer> _allocate_buffer(size_t size) = 0;
        [[nodiscard]] virtual std::shared_ptr<Texture> _allocate_texture(uint32_t width, uint32_t height, PixelFormat format) = 0;
        [[nodiscard]] virtual std::shared_ptr<Kernel> _compile_kernel(const dsl::Function &function) = 0;

        virtual void _launch(const std::function<void(Dispatcher &)> &dispatch) = 0;

    public:
//        explicit Device(Context *context) noexcept: _context{context} {}
//        virtual ~Device() noexcept = default;
//
//        [[nodiscard]] Context &context() const noexcept { return *_context; }

        template<typename Def, std::enable_if_t<std::is_invocable_v<Def>, int> = 0>
        [[nodiscard]] KernelView compile_kernel(std::string name, Def &&def) {
            auto function = std::make_shared<dsl::Function>(std::move(name));
            dsl::Function::push(function.get());
            def();
            dsl::Function::pop(function.get());
            return KernelView{std::async(std::launch::async, [function = std::move(function), this] { return _compile_kernel(*function); })};
        }
    };
}