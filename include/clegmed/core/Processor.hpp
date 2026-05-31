//
// Created by Michael Repplinger on 29.05.26.
//

#pragma once
#include <iostream>

#include "OutputPipe.hpp"
#include "Traits.hpp"

namespace clegmed::core {
    template<typename InputData, typename OutputData, typename Strategy>
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
            auto result = m_strategy(std::move(input_data));
            m_output_pipe.forward(std::move(result));
        }

    private:
        Strategy m_strategy;
        OutputPipe<OutputData> m_output_pipe = OutputPipe<OutputData>(*this);
    };


    template <typename ProcessorStrategy>
    Processor(ProcessorStrategy) -> Processor<
        typename detail::function_traits<decltype(&ProcessorStrategy::operator())>::argument_type,
        typename detail::function_traits<decltype(&ProcessorStrategy::operator())>::result_type,
        ProcessorStrategy>;

}
