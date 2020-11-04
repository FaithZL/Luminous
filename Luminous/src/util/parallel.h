//
// Created by Zero on 2020/11/3.
//

#pragma once

#include "core/header.h"
#include <atomic>
#include <functional>
#include <future>
#include "core/math/data_types.h"

namespace luminous {
    inline namespace utility {

        class AtomicFloat {
        private:
            std::atomic<float> val;

        public:
            using Float = float;
            explicit AtomicFloat(Float v = 0) : val(v) {}

            AtomicFloat(const AtomicFloat &rhs) : val((float)rhs.val) {}

            void add(Float v) {
                auto current = val.load();
                while (!val.compare_exchange_weak(current, current + v)) {
                }
            }

            [[nodiscard]] float value() const { return val.load(); }

            explicit operator float() const { return value(); }

            void set(Float v) { val = v; }
        };

        void parallel_for(int count, const std::function<void(uint32_t, uint32_t)> &func, size_t chunkSize = 1);
        [[nodiscard]] size_t num_work_threads();
        inline void parallel_for_2d(const int2 &dim, const std::function<void(int2, uint32_t)> &func,
                                    size_t chunkSize = 1) {
            parallel_for(
                    dim.x * dim.y,
                    [&](uint32_t idx, int tid) {
                        auto x = idx % dim.x;
                        auto y = idx / dim.x;
                        func(make_int2(x, y), tid);
                    },
                    chunkSize);
        }

        
    }
}