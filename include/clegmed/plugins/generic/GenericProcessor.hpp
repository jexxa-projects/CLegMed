#pragma once
#include <concepts>
#include <string>
#include <vector>
#include "../../core/Processor.hpp"
#include "../../utils/Logger.hpp"

namespace clegmed::plugins::generic {

    template<typename TInput, typename TSuffix>
    concept CanAppend =
    (
        requires(TInput& input, TSuffix&& suffix) { { input += std::forward<TSuffix>(suffix) }; } ||
        std::convertible_to<TSuffix, TInput>
    )
    &&!std::is_arithmetic_v<std::decay_t<TInput>> && !std::is_arithmetic_v<std::decay_t<TSuffix>>;

    template<typename TInput = void, typename TSuffix>
    [[nodiscard]] auto append(TSuffix suffix) {

        using DeducedInput = std::conditional_t<
            std::is_same_v<std::decay_t<TSuffix>, const char*>,
            std::string,
            std::decay_t<TSuffix>
        >;
        using ActualInputType = std::conditional_t<std::is_same_v<TInput, void>, DeducedInput, TInput>;

        static_assert(CanAppend<ActualInputType, TSuffix>,  "Given type does not support append!");

        auto lambda_strategy = [suffix = std::move(suffix)](ActualInputType input_data) -> ActualInputType {
            input_data += suffix;
            return input_data;
        };

        return core::make_processor(lambda_strategy);
    }

    struct PassThroughFactory {
        template <typename InputData>
        [[nodiscard]] auto build() const {
            auto lambda_strategy = [](const InputData& input_data) -> InputData {
                return input_data;
            };
            return core::make_processor(std::move(lambda_strategy));
        }
    };

    /**
     * Passthrough filter
     * @tparam InputData
     * @return
     */
    [[nodiscard]] inline auto passThrough() {
        return PassThroughFactory{};
    }


    template<typename T = std::string>
    [[nodiscard]] auto traceInfo() {
        return core::make_processor([](T data) {
            utils::Logger::log(utils::LogLevel::INFO, "{}", data);
            return data;
        });
    }
    template<typename T = std::string>
    [[nodiscard]] auto traceWarn() {
        return core::make_processor([](T data) {
            utils::Logger::log(utils::LogLevel::WARN, "{}", data);
            return data;
        });
    }

    template<typename T = std::string>
    [[nodiscard]] auto traceError() {
        return core::make_processor([](T data) {
            utils::Logger::log(utils::LogLevel::ERROR, "{}", data);
            return data;
        });
    }

}

