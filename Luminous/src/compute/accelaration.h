//
// Created by Zero on 2020/9/27.
//

#pragma once

#include <compute/dispatcher.h>
#include <compute/buffer.h>
#include <compute/ray.h>
#include <compute/hit.h>

namespace luminous::compute {
    class Acceleration {

    private:
        virtual void _refit(Dispatcher &dispatch) = 0;

        virtual void _intersect_any(Dispatcher &dispatch,
                                    const BufferView<Ray> &ray_buffer,
                                    const BufferView<AnyHit> &hit_buffer) const = 0;

        virtual void _intersect_closest(Dispatcher &dispatch,
                                        const BufferView<Ray> &ray_buffer,
                                        const BufferView<ClosestHit> &hit_buffer) const = 0;

    public:
        virtual ~Acceleration() noexcept = default;
        [[nodiscard]] auto refit() { return [this](Dispatcher &dispatch) { _refit(dispatch); }; }

        // size of ray_buffer implies dispatch size
        [[nodiscard]] auto intersect_any(BufferView<Ray> ray_buffer,
                                         BufferView<AnyHit> hit_buffer) const {
            auto ret = [this, ray_buffer = std::move(ray_buffer), hit_buffer = std::move(hit_buffer)](Dispatcher &dispatch) {
                _intersect_any(dispatch, ray_buffer, hit_buffer);
            };
            return ret;
        }

        // size of ray_buffer implies dispatch size
        [[nodiscard]] auto intersect_closest(BufferView<Ray> ray_buffer,
                                             BufferView<ClosestHit> hit_buffer) const {
            auto ret = [this, ray_buffer = std::move(ray_buffer), hit_buffer = std::move(hit_buffer)](Dispatcher &dispatch) {
                _intersect_closest(dispatch, ray_buffer, hit_buffer);
            };
            return ret;
        }
    };
}