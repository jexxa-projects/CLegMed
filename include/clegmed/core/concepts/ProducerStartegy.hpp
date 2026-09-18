#pragma once
#include <concepts>
#include "InjectedStrategy.hpp"

namespace clegmed::core {

/**
 * @brief Producer variant that generates data and returns it directly.
 */
template <typename S, typename Out>
concept ProducerWithDirectReturn =
    requires(S&& strategy) {
        { strategy() } -> std::same_as<Out>;
    };

/**
 * @brief Producer variant that generates data and pushes it into an output pipe.
 */
template <typename S, typename Out>
concept ProducerWithPipe =
    requires(S&& strategy, OutputPipe<Out>& pipe) {
        { strategy(pipe) } -> std::same_as<void>;
    };

/**
 * @brief Producer variant that uses external properties and returns the generated data.
 */
template <typename S, typename Out, typename Props>
concept ProducerWithPropertiesAndReturn =
    utils::DeserializableFromProperties<Props> &&
    requires(S&& strategy, Props& props) {
        { strategy(props) } -> std::same_as<Out>;
    };

/**
 * @brief Producer variant that uses external properties and pushes data into a pipe.
 */
template <typename S, typename Out, typename Props>
concept ProducerWithPropertiesAndPipe =
    utils::DeserializableFromProperties<Props> &&
    requires(S&& strategy, OutputPipe<Out>& pipe, Props& props) {
        { strategy(pipe, props) } -> std::same_as<void>;
    };


// ============================================================================
// 3. MAIN FRAMEWORK COMBINATOR CONCEPT
// ============================================================================

/**
 * @brief Validates that a type qualifies as a valid data producer strategy.
 *
 * The type must support constructor injection requirements (InjectedStrategy)
 * and fulfill exactly one of the supported generation signatures.
 */
template <
    typename Strategy,
    typename OutputData,
    typename FilterProperties
>
concept ValidProducerStrategy =
    InjectedStrategy<Strategy> && // <--- VISUAL ANCHOR: Constructor Injection Verified
    (
        ProducerWithDirectReturn<Strategy, OutputData> ||
        ProducerWithPipe<Strategy, OutputData> ||
        ProducerWithPropertiesAndReturn<Strategy, OutputData, FilterProperties> ||
        ProducerWithPropertiesAndPipe<Strategy, OutputData, FilterProperties>
    );

}