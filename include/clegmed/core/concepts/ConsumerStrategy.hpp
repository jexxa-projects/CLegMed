#pragma once

#include <concepts>
#include <utility>
#include "InjectedStrategy.hpp"

namespace clegmed::core {
    /**
     * @brief Consumer variant that processes input data directly and returns nothing.
     */
    template <typename S, typename In>
    concept ConsumerDirect =
        requires(S&& strategy, In&& input) {
        { strategy(std::forward<In>(input)) } -> std::same_as<void>;
        };

    /**
     * @brief Consumer variant that processes input data utilizing active runtime properties.
     */
    template <typename S, typename In, typename Props>
    concept ConsumerWithProperties =
        utils::DeserializableFromProperties<Props> &&
        requires(S&& strategy, In&& input, const Props& props) {
        { strategy(std::forward<In>(input), props) } -> std::same_as<void>;
        };


    /**
     * @brief Validates that a type qualifies as a valid data consumer strategy.
     *
     * The type must support constructor injection requirements (InjectedStrategy)
     * and fulfill exactly one of the supported consumption signatures.
     */
    template <
        typename Strategy,
        typename InputData,
        typename FilterProperties
    >
    concept ValidConsumerStrategy =
        InjectedStrategy<Strategy> &&
        (
            ConsumerDirect<Strategy, InputData> ||
            ConsumerWithProperties<Strategy, InputData, FilterProperties>
        );
}