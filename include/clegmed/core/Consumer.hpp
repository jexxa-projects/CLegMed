//
// Created by Michael Repplinger on 31.05.26.
//

#pragma once
#include "Filter.hpp"
#include "Traits.hpp"

namespace clegmed::core {
    template <typename InputData, typename ConsumerStrategy>
    class Consumer : public Filter {
    public:
        Consumer() = delete;
        explicit Consumer(ConsumerStrategy strategy) : m_strategy(strategy) {}
        ~Consumer() override = default;

        auto inputPipe() {
            return [this](InputData data) { this->consume(std::move(data)); };
        }
        void consume(InputData input_data) {
            m_strategy(std::move(input_data));
        }
    private:
        ConsumerStrategy m_strategy;
    };

    template <typename ConsumerStrategy>
    Consumer(ConsumerStrategy) -> Consumer<
    typename detail::function_traits<decltype(&ConsumerStrategy::operator())>::argument_type,
    ConsumerStrategy>;
}
