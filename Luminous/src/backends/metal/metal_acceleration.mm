//
// Created by Zero on 2020/9/29.
//

#import "metal_acceleration.h"
#import "metal_dispatcher.h"
#import "metal_buffer.h"

namespace luminous::metal {

    using namespace compute;

    void MetalAcceleration::_intersect_any(compute::Dispatcher &dispatch,
                                           const BufferView<Ray> &ray_buffer,
                                           const BufferView<AnyHit> &hit_buffer) const {

        [_any_intersector encodeIntersectionToCommandBuffer:dynamic_cast<MetalDispatcher &>(dispatch).handle()
                                           intersectionType:MPSIntersectionTypeAny
                                                  rayBuffer:dynamic_cast<MetalBuffer *>(ray_buffer.buffer())->handle()
                                            rayBufferOffset:ray_buffer.byte_offset()
                                         intersectionBuffer:dynamic_cast<MetalBuffer *>(hit_buffer.buffer())->handle()
                                   intersectionBufferOffset:hit_buffer.byte_offset()
                                                   rayCount:ray_buffer.size()
                                      accelerationStructure:_as];
    }

    void MetalAcceleration::_refit(compute::Dispatcher &dispatch) {
        auto command_buffer = dynamic_cast<MetalDispatcher &>(dispatch).handle();
        [_as encodeRefitToCommandBuffer:command_buffer];
    }

    void MetalAcceleration::_intersect_closest(compute::Dispatcher &dispatch,
                                               const BufferView<Ray> &ray_buffer,
                                               const BufferView<ClosestHit> &hit_buffer) const {

        [_closest_intersector encodeIntersectionToCommandBuffer:dynamic_cast<MetalDispatcher &>(dispatch).handle()
                                               intersectionType:MPSIntersectionTypeNearest
                                                      rayBuffer:dynamic_cast<MetalBuffer *>(ray_buffer.buffer())->handle()
                                                rayBufferOffset:ray_buffer.byte_offset()
                                             intersectionBuffer:dynamic_cast<MetalBuffer *>(hit_buffer.buffer())->handle()
                                       intersectionBufferOffset:hit_buffer.byte_offset()
                                                       rayCount:ray_buffer.size()
                                          accelerationStructure:_as];
    }

}