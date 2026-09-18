#pragma once

#include  <concepts>

#include "InjectedStrategy.hpp"

namespace clegmed::core {
    // ============================================================================
    // CALLABLE INTERFACE BLOCKS (SIGNATURE VARIANTS)
    // ============================================================================

    /**
     * @brief Strategy variant that processes data and returns the result directly.
     */
    template <typename S, typename In, typename Out>
    concept StrategyWithDirectReturn =
        requires(S&& strategy, const In& input) {
        { strategy(input) } -> std::convertible_to<Out>;
        };

    /**
     * @brief Strategy variant that processes data and passes it to an output pipe.
     */
    template <typename S, typename In, typename Out>
    concept StrategyWithPipe =
        requires(S&& strategy, const In& input, OutputPipe<Out>& pipe) {
        { strategy(input, pipe) } -> std::same_as<void>;
        };

    /**
     * @brief Strategy variant that uses external properties and returns the result directly.
     */
    template <typename S, typename In, typename Out, typename Props>
    concept StrategyWithPropertiesAndReturn =
        utils::DeserializableFromProperties<Props> &&
        requires(S&& strategy, const In& input, const Props& props) {
        { strategy(input, props) } -> std::convertible_to<Out>;
        };

    /**
     * @brief Strategy variant that uses external properties and passes data to an output pipe.
     */
    template <typename S, typename In, typename Out, typename Props>
    concept StrategyWithPropertiesAndPipe =
        utils::DeserializableFromProperties<Props> &&
        requires(S&& strategy, const In& input, OutputPipe<Out>& pipe, const Props& props) {
        { strategy(input, pipe, props) } -> std::same_as<void>;
        };


    // ============================================================================
    // MAIN FRAMEWORK COMBINATOR CONCEPT
    // ============================================================================

    /**
     * @brief Validates that a type qualifies as a processor strategy.
     *
     * The type must support constructor injection requirements (InjectedStrategy)
     * and fulfill exactly one of the supported execution signatures.
     */
    template <
        typename Strategy,
        typename InputData,
        typename OutputData,
        typename FilterProperties
    >
    concept ValidProcessorStrategy =
        InjectedStrategy<Strategy> &&
        (
            StrategyWithDirectReturn<Strategy, InputData, OutputData> ||
            StrategyWithPipe<Strategy, InputData, OutputData> ||
            StrategyWithPropertiesAndReturn<Strategy, InputData, OutputData, FilterProperties> ||
            StrategyWithPropertiesAndPipe<Strategy, InputData, OutputData, FilterProperties>
        );
}
