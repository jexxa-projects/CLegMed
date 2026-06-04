//
// Created by Michael Repplinger on 29.05.26.
//

#pragma once
#include <iostream>

#include "OutputPipe.hpp"
#include "Traits.hpp"

namespace clegmed::core {

    template <typename Strategy, typename InputData, typename OutputData>
    concept ValidProcessorStrategy =
        std::is_invocable_r_v<void, Strategy, const InputData&, OutputPipe<OutputData>&> ||
        std::is_invocable_r_v<OutputData, Strategy, const InputData&>;


    template<typename InputData, typename OutputData, typename Strategy>
        requires ValidProcessorStrategy<Strategy, InputData, OutputData>
    class Processor : public Filter {
    public:
        Processor() = delete;
        explicit  Processor(Strategy strategy) : m_strategy(std::move(strategy)) {}
        ~Processor() override = default;

        auto inputPipe() {
            return [this](InputData data) { this->process(std::move(data)); };
        }

        template<typename Self>
        auto&& outputPipe(this Self&& explicit_this) {
            return std::forward<Self>(explicit_this).m_output_pipe;
        }

        void process(InputData input_data) {
            if constexpr (std::is_invocable_v<Strategy, const InputData&, OutputPipe<OutputData>&>) {
                m_strategy(std::move(input_data), m_output_pipe);
            } else if constexpr (std::is_invocable_r_v<OutputData, Strategy, const InputData&>) {
                auto result = m_strategy(std::move(input_data));
                m_output_pipe.forward(std::move(result));
            } else {
                static_assert(false,
                    "❌ ARCHITECTURE-ERROR: Given ProcessStrategy neither uses "
                    "Piped-Signature (Input, Pipe&) nor 1:1-signature (Input).");
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
