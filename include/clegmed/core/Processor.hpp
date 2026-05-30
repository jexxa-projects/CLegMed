//
// Created by Michael Repplinger on 29.05.26.
//

#pragma once
#include <iostream>

#include "OutputPipe.hpp"

namespace clegmed::core {
    template<typename InputData>
    class Processor : public Filter {
    public:
        Processor() = default;
        ~Processor() override = default;

        auto inputPipe() {
            return [this](InputData data) { this->process(std::move(data)); };
        }

        template<typename Self>
        auto&& outputPipe(this Self&& explicit_this) {
            return std::forward<Self>(explicit_this).m_output_pipe;
        }

        virtual void process(InputData input_data) {
            m_output_pipe.forward(std::move(input_data));
        }

    private:
        OutputPipe<InputData> m_output_pipe = OutputPipe<InputData>(*this);
    };
}
