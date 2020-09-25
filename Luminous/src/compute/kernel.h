//
// Created by Zero on 2020/9/8.
//

#pragma once

#include <vector>
#include <tuple>
#include <future>

#include <core/math/data_types.h>
#include <compute/buffer.h>

namespace luminous::compute {
    class Dispatcher;

    class Kernel : public Noncopyable {

    public:
        struct Resource {
            std::shared_ptr<Buffer> buffer{nullptr};
            std::shared_ptr<Texture> texture{nullptr};
        };

        struct Uniform {
            std::vector<std::byte> immutable;
            const void *binding{nullptr};
            size_t binding_size{};
            size_t offset{0u};
        };

        friend class KernelView;

    protected:
        std::vector<Resource> _resources;
        std::vector<Uniform> _uniforms;
        virtual void _dispatch(Dispatcher &dispatcher, uint2 blocks, uint2 block_size) = 0;

    public:
        Kernel(std::vector<Resource> resources, std::vector<Uniform> uniforms) noexcept
                : _resources{std::move(resources)}, _uniforms{std::move(uniforms)} {}
        virtual ~Kernel() noexcept = default;
    };

    class KernelView {

    private:
        std::shared_future<std::shared_ptr<Kernel>> _kernel;

    public:
        KernelView() noexcept = default;
        explicit KernelView(std::shared_future<std::shared_ptr<Kernel>> kernel) noexcept : _kernel{std::move(kernel)} {}

        [[nodiscard]] auto parallelize(uint threads, uint block_size = 256u) {
            return [self = _kernel, threads, block_size](Dispatcher &dispatch) {
                self.get()->_dispatch(dispatch, make_uint2((threads + block_size - 1u) / block_size, 1u), make_uint2(block_size, 1u));
            };
        }

        [[nodiscard]] auto parallelize(uint2 threads, uint2 block_size = make_uint2(16u, 16u)) {
            return [self = _kernel, threads, block_size](Dispatcher &dispatch) {
                self.get()->_dispatch(dispatch, (threads + block_size - 1u) / block_size, block_size);
            };
        }

        [[nodiscard]] bool empty() const noexcept { return !_kernel.valid(); }
    };
}