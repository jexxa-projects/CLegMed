#pragma once
#include <concepts>

namespace clegmed::core {
    /**
     * @brief Defines the lifecycle requirements for an injected strategy.
     *
     * This concept ensures that the strategy has already been constructed
     * (e.g., via Constructor Injection by a DI container or factory)
     * and is ready to be moved into the processing pipeline.
     */
    template <typename T>
    concept InjectedStrategy = std::move_constructible<T> && std::destructible<T>;
}