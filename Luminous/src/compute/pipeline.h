//
// Created by Zero on 2020/10/26.
//

#pragma once

#include <compute/device.h>
#include <compute/kernel.h>
#include <compute/dispatcher.h>

namespace luminous::compute {

    class Pipeline : public Noncopyable {
    public:
        static constexpr auto max_stages_in_queue = 32u;
    private:
        Device *_device;
        std::queue<std::function<void(Dispatcher &)>> _stages;
    public:
        explicit Pipeline(Device *device) : _device{device} {}

        [[nodiscard]] Device *device() const noexcept { return _device; }

        template<typename Func,
                std::enable_if_t<std::is_invocable_v<Func, Dispatcher &>, int> = 0>
        Pipeline &operator<<(Func &&func) {
            _stages.emplace(std::forward<Func>(func));
            if (_stages.size() >= max_stages_in_queue) {
                run();
            }
            return *this;
        }

        template<typename Func,
                std::enable_if_t<std::is_invocable_v<Func>, int> = 0>
        Pipeline &operator<<(Func &&func) {
            _stages.emplace([f = std::forward<Func>(func)](Dispatcher &) { f(); });
            if (_stages.size() >= max_stages_in_queue) {
                run();
            }
            return *this;
        }

        template<typename Func,
                std::enable_if_t<std::is_invocable_v<Func, Pipeline &>, int> = 0>
        Pipeline &operator<<(Func &&func) {
            func(*this);
            return *this;
        }

        void run() {
            _device->launch([this](Dispatcher &dispatch) {
                while (!_stages.empty()) {
                    dispatch.exec(_stages.front());
                    _stages.pop();
                }
            });
        }

        void synchronize() noexcept {
            run();
            _device->synchronize();
        }
    };

}