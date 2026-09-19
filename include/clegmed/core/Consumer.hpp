#pragma once
#include <memory>
#include <exception>
#include "Filter.hpp"
#include "Traits.hpp"
#include "clegmed/utils/Logger.hpp"
#include "concepts/ConsumerStrategy.hpp"

namespace clegmed::core {
    template <typename InputData, typename ConsumerStrategy, typename FilterProperties = std::monostate>
        requires ValidConsumerStrategy<ConsumerStrategy, InputData, FilterProperties>
    class Consumer : public Filter {
        FilterProperties m_properties;
    public:
        explicit Consumer(ConsumerStrategy strategy) : m_strategy(strategy) {}

        void properties(const utils::Properties& properties) {
            if constexpr (!std::is_same_v<FilterProperties, std::monostate>) {
                if (hasProperties()) {
                    m_properties = properties.get<FilterProperties>(propertiesName());
                }
            }
        }


        auto inputPipe() {
            return [this]<typename T> requires std::is_convertible_v<T, InputData> (T&& data) {
                this->consume(std::forward<T>(data));
            };
        }

        template<typename T>
        requires std::is_convertible_v<T, InputData>
        void consume(T&& input_data) {
            const PipelineResult pipeline_result = executeStrategy(std::forward<T>(input_data));

            validate(pipeline_result);
        }


    private:
        using PipelineResult = std::optional<std::expected<void, std::exception_ptr>>;

        template <typename T>
        [[nodiscard]] PipelineResult executeStrategy(T&& input_data) noexcept
        {
            if constexpr (ConsumerDirect<ConsumerStrategy, InputData>) {
                m_strategy(std::forward<T>(input_data));
                return PipelineResult{ std::expected<void, std::exception_ptr>{} };
            }
            else if constexpr ( ConsumerWithProperties<ConsumerStrategy, InputData, FilterProperties> ) {
                m_strategy(std::forward<T>(input_data), m_properties);
                return PipelineResult{ std::expected<void, std::exception_ptr>{} };
            }
            else {
                static_assert(false,"❌ ARCHITECTURE-ERROR: Unsupported consumer strategy signature.");
            }

            return PipelineResult{ std::nullopt };
        }

        static void validate(const PipelineResult& pipeline_result) {
            if (!pipeline_result.has_value()) { // is null-opt
                return;
            }

            if (pipeline_result->has_value()) { //pointer oder null
                return;
            }

            //TODO: Actually, we do not know how to handle errors in general
            if (pipeline_result->error()) {
                utils::Logger::log(utils::LogLevel::ERROR,

                    "Pipeline Error: error occurred . TODO implement error handling"
                    );
                exit(EXIT_FAILURE);
            }
        }

        [[no_unique_address]] ConsumerStrategy m_strategy;
    };

    template<typename ConsumerStrategy>
    [[nodiscard]] auto make_consumer(ConsumerStrategy&& strategy) {
        using DecayedStrategy = std::decay_t<ConsumerStrategy>;
        using MemberPtr = decltype(&DecayedStrategy::operator());
        using InputData = detail::function_traits<MemberPtr>::template argument_t<0>;

        using ConcreteConsumer = Consumer<InputData, DecayedStrategy>;

        return std::make_unique<ConcreteConsumer>(
            std::forward<ConsumerStrategy>(strategy)
        );
    }

    template<typename ConsumerStrategy>
    [[nodiscard]] auto make_configured_consumer(ConsumerStrategy&& strategy) {
        using DecayedStrategy = std::decay_t<ConsumerStrategy>;
        using MemberPtr = decltype(&DecayedStrategy::operator());

        using InputData = detail::function_traits<MemberPtr>::template argument_t<0>;
        using FilterProperties = detail::function_traits<MemberPtr>::template argument_t<1>;

        using ConcreteConsumer = Consumer<InputData, DecayedStrategy, FilterProperties>;

        return std::make_unique<ConcreteConsumer>(
            std::forward<ConsumerStrategy>(strategy)
        );
    }

}
