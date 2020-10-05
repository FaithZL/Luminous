//
// Created by Zero on 2020/9/6.
//

#import <cstdlib>
#import <queue>
#import <map>

#import <MetalPerformanceShaders/MetalPerformanceShaders.h>

#import <core/hash.h>

#import <compute/device.h>
#import <compute/acceleration.h>

#import "metal_buffer.h"
#import "metal_texture.h"
#import "metal_kernel.h"
#import "metal_codegen.h"
#import "metal_dispatcher.h"
#import "metal_acceleration.h"

namespace luminous::metal {
    using namespace compute;

    class MetalDevice : public Device {

    public:
        static constexpr auto max_command_queue_size = 16u;

    private:
        id<MTLDevice> _handle;
        id<MTLCommandQueue> _command_queue;
        std::mutex _kernel_cache_mutex;
        std::map<SHA1::Digest, id<MTLComputePipelineState>> _kernel_cache;
        std::vector<std::unique_ptr<MetalDispatcher>> _dispatchers;
        uint32_t _next_dispatcher{0u};

        [[nodiscard]] MetalDispatcher &_get_next_dispatcher() noexcept {
            auto id = _next_dispatcher;
            _next_dispatcher = (_next_dispatcher + 1u) % max_command_queue_size;
            auto &&dispatcher = *_dispatchers[id];
            dispatcher.wait();
            return dispatcher;
        }

    protected:
        std::shared_ptr<Kernel> _compile_kernel(const compute::dsl::Function &f) override;
        std::shared_ptr<Buffer> _allocate_buffer(size_t size) override;
        std::shared_ptr<Texture> _allocate_texture(uint32_t width, uint32_t height, compute::PixelFormat format) override;
        void _launch(const std::function<void(Dispatcher &)> &dispatch) override;

    public:
        explicit MetalDevice(Context *context, uint32_t device_id);

        ~MetalDevice() noexcept override = default;

        void synchronize() override;

        std::unique_ptr<Acceleration> build_acceleration(
                const BufferView<float3> &positions,
                const BufferView<TriangleHandle> &indices,
                const std::vector<MeshHandle> &meshes,
                const BufferView<uint> &instances,
                const BufferView<float4x4> &transforms,
                bool is_static) override;
    };

    // implement
    MetalDevice::MetalDevice(Context *context, uint32_t device_id) : Device{context, device_id} {
        auto devices = MTLCopyAllDevices();
        LUMINOUS_ERROR_IF_NOT(device_id < devices.count, "Invalid Metal device index ",
                              device_id, ": max available index is ", devices.count - 1, ".");
        _handle = devices[device_id];
        LUMINOUS_INFO("Created Metal device #", device_id, ", description:\n", [_handle.description cStringUsingEncoding:NSUTF8StringEncoding]);
        _command_queue = [_handle newCommandQueueWithMaxCommandBufferCount:max_command_queue_size];
        _dispatchers.reserve(max_command_queue_size);
        for (auto i = 0u; i < max_command_queue_size; i++) {
            _dispatchers.emplace_back(std::make_unique<MetalDispatcher>());
        }
    }

    std::shared_ptr<Kernel> MetalDevice::_compile_kernel(const compute::dsl::Function &f) {

    }

    std::shared_ptr<Buffer> MetalDevice::_allocate_buffer(size_t size) {
        auto buffer = [_handle newBufferWithLength:size options:MTLResourceStorageModePrivate];
        return std::make_shared<MetalBuffer>(buffer, size);
    }

    std::shared_ptr<Texture> MetalDevice::_allocate_texture(uint32_t width,
                                                            uint32_t height,
                                                            compute::PixelFormat format) {
        auto desc = [[MTLTextureDescriptor alloc] init];
        desc.textureType = MTLTextureType2D;
        desc.width = width;
        desc.height = height;
        desc.allowGPUOptimizedContents = true;
        desc.storageMode = MTLStorageModePrivate;
        switch (format) {
            case PixelFormat::R8U:
                desc.pixelFormat = MTLPixelFormatA8Unorm;
                break;
            case PixelFormat::RG8U:
                desc.pixelFormat = MTLPixelFormatRG8Unorm;
                break;
            case PixelFormat::RGBA8U:
                desc.pixelFormat = MTLPixelFormatRGBA8Unorm;
                break;
            case PixelFormat::R32F:
                desc.pixelFormat = MTLPixelFormatR32Float;
                break;
            case PixelFormat::RG32F:
                desc.pixelFormat = MTLPixelFormatRG32Float;
                break;
            case PixelFormat::RGBA32F:
                desc.pixelFormat = MTLPixelFormatRGBA32Float;
                break;
        }
        auto texture = [_handle newTextureWithDescriptor:desc];
        return std::make_shared<MetalTexture>(texture, width, height, format);
    }

    void MetalDevice::_launch(const std::function<void(Dispatcher &)> &dispatch) {
        auto &&dispatcher = _get_next_dispatcher();
        auto command_buffer = [_command_queue commandBufferWithUnretainedReferences];
        dispatcher.reset(command_buffer);
        dispatch(dispatcher);
        dispatcher.commit();
    }

    void MetalDevice::synchronize() {
        for (auto i = 0u; i < max_command_queue_size; i++) {
            _get_next_dispatcher().reset();
        }
    }

    std::unique_ptr<Acceleration> MetalDevice::build_acceleration(const BufferView<float3> &positions,
                                                                  const BufferView<TriangleHandle> &indices,
                                                                  const std::vector<MeshHandle> &meshes,
                                                                  const BufferView<uint> &instances,
                                                                  const BufferView<float4x4> &transforms,
                                                                  bool is_static) {

    }

}

