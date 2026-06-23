#pragma once
#include <concepts>
namespace clegmed::core {
    template <typename F, typename T>
    concept InputPipe = std::invocable<F, T>;
} // clegmed

