//
// Created by Zero on 2020/10/1.
//

#pragma once

#import <compute/buffer.h>
#import "metal_host_cache.h"

namespace luminous::metal {
    using compute::Buffer;
    using compute::Dispatcher;

    class MetalBuffer : public Buffer {

    private:
        id<MTLBuffer> _handle{nullptr};
        MetalHostCache _cache;

    public:
        MetalBuffer(id<MTLBuffer> buffer, size_t size) noexcept
                : Buffer{size},
                _handle{buffer},
                _cache{[buffer device], size} {

        }

        [[nodiscard]] id<MTLBuffer> handle() const noexcept {
            return _handle;
        }

        void upload(compute::Dispatcher &dispatcher,
                    size_t offset, size_t size,
                    const void *host_data) override;

        void download(compute::Dispatcher &dispatcher,
                      size_t offset, size_t size,
                      void *host_buffer) override;

        void clear_cache() noexcept override {
            _cache.clear();
        }

        void with_cache(compute::Dispatcher &dispatch,
                        const std::function<void(void *)> &modify,
                        size_t offset, size_t length) override;
    };
}