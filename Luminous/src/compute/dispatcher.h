//
// Created by Zero on 2020/9/6.
//

#pragma once

#include <type_traits>
#include <vector>
#include <thread>
#include <future>
#include <functional>
#include <core/CObject.h>
#include <math/data_types.h>

namespace luminous::compute {

class Device;
class Kernel;

class Dispatcher : Noncopyable {

public:
    friend class Device;

protected:
    std::vector<std::function<void()>> _callbacks;

    virtual void _on_dispatch() {

    };
public:

    Dispatcher() noexcept = default;
    Dispatcher(Dispatcher &&) noexcept = default;
    Dispatcher &operator=(Dispatcher &&) noexcept = default;

    virtual ~Dispatcher() noexcept = default;

    virtual void commit() = 0;
    virtual void wait() = 0;

    template<typename Callback, std::enable_if_t<std::is_invocable_v<Callback>, int> = 0>
    void when_completed(Callback &&f) noexcept { _callbacks.emplace_back(std::forward<Callback>(f)); }

    template<typename F, std::enable_if_t<std::is_invocable_v<F, Dispatcher &>, int> = 0>
    void operator()(F &&f) {
        f(*this);
        _on_dispatch();
    }

    template<typename F, typename CB,
            std::enable_if_t<std::conjunction_v<std::is_invocable<F, Dispatcher &>, std::is_invocable<CB>>, int> = 0>
    void operator()(F &&f, CB &&callback) {
        f(*this);
        when_completed(std::forward<CB>(callback));
        _on_dispatch();
    }
};


}