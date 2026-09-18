#pragma once
#include <expected>
#include <exception>

#include "OutputPipe.hpp"
#include "Traits.hpp"
#include "clegmed/utils/Properties.hpp"
#include "concepts/ProcessorStrategy.hpp"

namespace clegmed::core {


    template<
        typename InputData,
        typename OutputData,
        typename Strategy,
        typename FilterProperties = std::monostate>
    requires ValidProcessorStrategy<Strategy, InputData, OutputData, FilterProperties>

    class Processor : public Filter {
    public:
        explicit  Processor(Strategy strategy) : m_strategy(std::move(strategy)) {}

        void properties(const utils::Properties& properties) {
            if constexpr (!std::is_same_v<FilterProperties, std::monostate>) {
                if (hasProperties()) {
                    m_properties = properties.get<FilterProperties>(propertiesName());
                }
            }
        }

        auto inputPipe() {
            return [this]<typename T>requires std::is_convertible_v<T, InputData>(T&& data)
                { this->process(std::forward<T>(data)); };
        }

        template<typename Self>
        auto&& outputPipe(this Self&& explicit_this) {
            return std::forward<Self>(explicit_this).m_output_pipe;
        }


        template<typename T> requires std::is_convertible_v<T, InputData>
        void process(T&& input_data) noexcept
        {
            PipelineResult pipeline_result = executeStrategy(input_data);

            validate(pipeline_result);

            forward(pipeline_result);
        }

    private:
        using PipelineResult = std::optional<std::expected<OutputData, std::exception_ptr>>;

        template <typename T>
        [[nodiscard]] PipelineResult executeStrategy(T&& input_data)
        {
            if constexpr (StrategyWithPropertiesAndReturn<Strategy, InputData, OutputData, FilterProperties>) {
                return PipelineResult{ m_strategy(std::forward<T>(input_data), m_properties) };
            }
            else if constexpr (StrategyWithDirectReturn<Strategy, InputData, OutputData>) {
                return PipelineResult{ m_strategy(std::forward<T>(input_data)) };
            }
            else if constexpr (StrategyWithPropertiesAndPipe<Strategy, InputData, OutputData, FilterProperties>) {
                m_strategy(std::forward<T>(input_data), m_output_pipe, m_properties);
                return PipelineResult{ std::nullopt }; // Already integrated via side-effect pipe
            }
            else if constexpr (StrategyWithPipe<Strategy, InputData, OutputData>) {
                m_strategy(std::forward<T>(input_data), m_output_pipe);
                return PipelineResult{ std::nullopt }; // Already integrated via side-effect pipe
            }
            else {
                static_assert([]{ return false; }(), "❌ ARCHITECTURE-ERROR: Unsupported strategy signature.");
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

        void forward(PipelineResult& pipeline_result) {
            pipeline_result.and_then([this](auto&& expected_val) {
                m_output_pipe.forward(std::move(*expected_val));
                return std::make_optional(true);
            });
            pipeline_result.reset();
        }


        [[no_unique_address]]
        Strategy m_strategy;
        FilterProperties m_properties;

        OutputPipe<OutputData> m_output_pipe = OutputPipe<OutputData>(*this);
    };


    template <typename ProcessorStrategy>
    [[nodiscard]] auto make_processor(ProcessorStrategy&& strategy) {
        using DecayedStrategy = std::decay_t<ProcessorStrategy>;
        using MemberPtr = decltype(&DecayedStrategy::operator());

        using InputData  = detail::function_traits<MemberPtr>::template argument_t<0>;
        using OutputData = detail::function_traits<MemberPtr>::result_type;

        using ConcreteProcessor = Processor<InputData, OutputData, DecayedStrategy>;

        return std::make_unique<ConcreteProcessor>(
            std::forward<ProcessorStrategy>(strategy)
        );
    }


    template <typename InputData, typename ProcessorStrategy>
    [[nodiscard]] auto make_processor(ProcessorStrategy&& strategy) {
        using DecayedStrategy = std::decay_t<ProcessorStrategy>;
        using ConcreteProcessor = Processor<InputData, InputData, DecayedStrategy>;

        return std::make_unique<ConcreteProcessor>(
            std::forward<ProcessorStrategy>(strategy)
        );
    }

    template <typename ProcessorStrategy>
    [[nodiscard]] auto make_piped_processor(ProcessorStrategy&& strategy) {
        using DecayedStrategy = std::decay_t<ProcessorStrategy>;
        using MemberPtr = decltype(&DecayedStrategy::operator());

        using InputData = detail::function_traits<MemberPtr>::template argument_t<0>;
        using PipeArg   = detail::function_traits<MemberPtr>::template argument_t<1>;
        using OutputData = detail::extract_pipe_type_t<PipeArg>;

        using ConcreteProcessor = Processor<InputData, OutputData, DecayedStrategy>;

        return std::make_unique<ConcreteProcessor>(
            std::forward<ProcessorStrategy>(strategy)
        );
    }

    template <typename ProcessorStrategy>
    [[nodiscard]] auto make_configured_piped_processor(ProcessorStrategy&& strategy) {
        using DecayedStrategy = std::decay_t<ProcessorStrategy>;
        using MemberPtr = decltype(&DecayedStrategy::operator());

        using InputData = detail::function_traits<MemberPtr>::template argument_t<0>;
        using PipeArg   = detail::function_traits<MemberPtr>::template argument_t<1>;
        using FilterProperties = detail::function_traits<MemberPtr>::template argument_t<2>;
        using OutputData = detail::extract_pipe_type_t<PipeArg>;

        using ConcreteProcessor = Processor<InputData, OutputData, DecayedStrategy, FilterProperties>;

        return std::make_unique<ConcreteProcessor>(
            std::forward<ProcessorStrategy>(strategy)
        );
    }


    template <typename ProcessorStrategy>
    [[nodiscard]]
    auto make_configured_processor(ProcessorStrategy&& strategy) {
        using DecayedStrategy = std::decay_t<ProcessorStrategy>;
        using MemberPtr = decltype(&DecayedStrategy::operator());

        using InputData = detail::function_traits<MemberPtr>::template argument_t<0>;
        using FilterProperties = detail::function_traits<MemberPtr>::template argument_t<1>;
        using OutputData = detail::function_traits<MemberPtr>::result_type;

        using ConcreteProcessor = Processor<InputData, OutputData, DecayedStrategy, FilterProperties>;

        return std::make_unique<ConcreteProcessor>(
            std::forward<ProcessorStrategy>(strategy)
        );
    }

}
