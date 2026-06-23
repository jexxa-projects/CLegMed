#pragma once
#include <iostream>
#include <expected>
#include <exception>

#include "OutputPipe.hpp"
#include "Traits.hpp"

namespace clegmed::core {

    template <typename Strategy, typename InputData, typename OutputData>
    concept ValidProcessorStrategy =
        requires(Strategy&& strategy, const InputData& input, OutputPipe<OutputData>& pipe)
        { { strategy(input, pipe) } -> std::same_as<void>;}
        ||
        requires(Strategy&& strategy, const InputData& input)
        { { strategy(input) } -> std::convertible_to<OutputData>;};


    template<typename InputData, typename OutputData, typename Strategy>
        requires ValidProcessorStrategy<Strategy, InputData, OutputData>
    class Processor : public Filter {
    public:
        Processor() = delete;
        explicit  Processor(Strategy strategy) : m_strategy(std::move(strategy)) {}
        ~Processor() override = default;
        Processor(Processor&&) noexcept = default;
        Processor& operator=(Processor&&) noexcept = default;
        auto inputPipe() {
            return [this](InputData data) { this->process(std::move(data)); };
        }

        template<typename Self>
        auto&& outputPipe(this Self&& explicit_this) {
            return std::forward<Self>(explicit_this).m_output_pipe;
        }

        void process(InputData input_data) noexcept
        {
            std::optional<std::expected<OutputData, std::exception_ptr>> pipeline_result;

            if constexpr (std::is_invocable_v<Strategy, const InputData&, OutputPipe<OutputData>&>) {
                return m_strategy(std::move(input_data), m_output_pipe);
            } else if constexpr (std::is_invocable_r_v<OutputData, Strategy, const InputData&>) {
                pipeline_result.emplace( m_strategy(std::move(input_data)) );
            } else {
                static_assert(false,
                    "❌ ARCHITECTURE-ERROR: Given ProcessStrategy neither uses "
                    "Piped-Signature (Input, Pipe&) nor 1:1-signature (Input).");
            }

            //TODO: Actually, we do not know how to handle errors in general
            if (!pipeline_result.has_value()) {
                std::cerr << "Pipeline Error: error occurred . TODO implement error handling"  << std::endl;
                exit(EXIT_FAILURE);
            }

            if constexpr (std::is_invocable_r_v<OutputData, Strategy, const InputData&>) {
                m_output_pipe.forward(std::move(*pipeline_result.value()));
            }
        }

    private:
        Strategy m_strategy;
        OutputPipe<OutputData> m_output_pipe = OutputPipe<OutputData>(*this);
    };


    template <typename ProcessorStrategy>
    [[nodiscard]] auto make_processor(ProcessorStrategy&& strategy) {
        using DecayedStrategy = std::decay_t<ProcessorStrategy>;
        using MemberPtr = decltype(&DecayedStrategy::operator());

        // Viel cleaner zu lesen:
        using InputData  = detail::function_traits<MemberPtr>::template argument_t<0>;
        using OutputData = detail::function_traits<MemberPtr>::result_type;

        return Processor<InputData, OutputData, DecayedStrategy>(
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

        return Processor<InputData, OutputData, DecayedStrategy>(
            std::forward<ProcessorStrategy>(strategy)
        );
    }


}
