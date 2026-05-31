//
// Created by Michael Repplinger on 31.05.26.
//

#pragma once
#include "Filter.hpp"
#include "OutputPipe.hpp"

namespace clegmed::core {
    template <typename OutputData, typename ProducerStrategy>
    class Producer : public Filter {
    public:
        Producer() = delete;
        ~Producer() override = default;

        explicit Producer(ProducerStrategy strategy) : Filter(), m_strategy(strategy) {}

        template<typename Self >
        auto&& outputPipe(this Self&& explicit_this) {
            return std::forward<Self>(explicit_this).m_outputPipe;
        }

        void produce() {
            auto result = m_strategy();
            m_outputPipe.forward(std::move(result));
        }

    private:
        ProducerStrategy m_strategy;
        OutputPipe<OutputData> m_outputPipe = OutputPipe<OutputData>(*this);
    };
}
