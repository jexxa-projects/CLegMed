//
// Created by Michael Repplinger on 31.05.26.
//

#pragma once
#include "Filter.hpp"
#include "Traits.hpp"

namespace clegmed::core {
    template<typename Strategy, typename InputData>
    concept ValidConsumerStrategy = std::is_invocable_r_v<void, Strategy, InputData>;

    template <typename InputData, typename ConsumerStrategy>
        requires ValidConsumerStrategy<ConsumerStrategy, InputData>
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

    template<typename ConsumerStrategy>
    [[nodiscard]] auto make_consumer(ConsumerStrategy&& strategy) {
        using DecayedStrategy = std::decay_t<ConsumerStrategy>;
        using MemberPtr = decltype(&DecayedStrategy::operator());
        using InputData = detail::function_traits<MemberPtr>::template argument_t<0>;

        return Consumer<InputData, std::decay_t<ConsumerStrategy>>(std::forward<ConsumerStrategy>(strategy));
    }


}
