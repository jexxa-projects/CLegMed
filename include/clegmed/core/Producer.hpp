//
// Created by Michael Repplinger on 31.05.26.
//

#pragma once
#include "Filter.hpp"
#include "OutputPipe.hpp"
#include <type_traits>
#include <utility>


namespace clegmed::core {
    template <typename Strategy, typename OutputData>
    concept ValidProducerStrategy =
    std::is_invocable_v<Strategy, OutputPipe<OutputData>&> ||
    std::is_invocable_v<Strategy>;

    template <typename OutputData, ValidProducerStrategy <OutputData> ProducerStrategy>
    class Producer : public Filter {
    public:
        Producer() = delete;
        ~Producer() override = default;

        explicit Producer(ProducerStrategy strategy) : Filter(), m_strategy(std::move(strategy)) {}

        template<typename Self >
        auto&& outputPipe(this Self&& explicit_this) {
            return std::forward<Self>(explicit_this).m_outputPipe;
        }

        void produce() {
            if constexpr (std::is_invocable_v<ProducerStrategy, OutputPipe<OutputData>&>) {
                m_strategy(m_outputPipe);
            } else if constexpr (std::is_invocable_v<ProducerStrategy>) {
                auto result = m_strategy();
                m_outputPipe.forward(std::move(result));
            }
        }

    private:
        ProducerStrategy m_strategy;
        OutputPipe<OutputData> m_outputPipe = OutputPipe<OutputData>(*this);
    };

    template <typename  ProducerStrategy>
    [[nodiscard]] auto producer(ProducerStrategy&& producer_strategy) {
        using OutputData = std::invoke_result_t<ProducerStrategy>;
        return Producer<OutputData, std::decay_t<ProducerStrategy>>(std::forward<ProducerStrategy>(producer_strategy));
    }

    template <typename OutputData, typename  ProducerStrategy>
    [[nodiscard]] auto pipedProducer(ProducerStrategy&& producer_strategy) {
        return Producer<OutputData, std::decay_t<ProducerStrategy>>(std::forward<ProducerStrategy>(producer_strategy));
    }

}
