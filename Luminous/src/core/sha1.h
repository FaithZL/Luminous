//
// Created by Zero on 2020/9/2.
//

#pragma once

#include <string>
#include <string_view>
#include <array>

class SHA1 {

public:
    using Digest = std::array<uint32_t, 5>;

private:
    Digest _digest{};
    std::string _buffer;
    uint64_t _transforms{};

public:
    explicit SHA1(std::string_view s);
    [[nodiscard]] auto digest() const noexcept { return _digest; }
};

