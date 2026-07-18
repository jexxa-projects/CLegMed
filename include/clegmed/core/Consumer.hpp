#pragma once
#include <iostream>
#include <ostream>

#include "Filter.hpp"
#include "Traits.hpp"

namespace clegmed::core {
    template<typename Strategy, typename InputData>
    concept ValidConsumerStrategy =
        requires(Strategy&& strategy, InputData&& input_data)
        { { strategy(std::forward<InputData>(input_data)) } -> std::same_as<void>; };

    template <typename InputData, typename ConsumerStrategy>
        requires ValidConsumerStrategy<ConsumerStrategy, InputData>
    class Consumer : public Filter {
    public:
        Consumer() = delete;
        explicit Consumer(ConsumerStrategy strategy) : m_strategy(strategy) {}
        ~Consumer() override = default;
        Consumer(Consumer&&) noexcept = default;
        Consumer& operator=(Consumer&&) noexcept = default;


        auto inputPipe() {
            return [this]<typename T> requires std::is_convertible_v<T, InputData> (T&& data) {
                this->consume(std::forward<T>(data));
            };
        }

        template<typename T>
        requires std::is_convertible_v<T, InputData>
        void consume(T&& input_data) {
            if constexpr (std::is_invocable_r_v<void, ConsumerStrategy, T&&>) {
                m_strategy(std::forward<T>(input_data));
            } else {
                static_assert(false,
                    "❌ ARCHITECTURE-ERROR: Given ConsumerStrategy does not use "
                    "1:1-signature (Input).");
            }
        }


    private:
        [[no_unique_address]] ConsumerStrategy m_strategy;
    };

    template<typename ConsumerStrategy>
    [[nodiscard]] auto make_consumer(ConsumerStrategy&& strategy) {
        using DecayedStrategy = std::decay_t<ConsumerStrategy>;
        using MemberPtr = decltype(&DecayedStrategy::operator());
        using InputData = detail::function_traits<MemberPtr>::template argument_t<0>;

        return Consumer<InputData, std::decay_t<ConsumerStrategy>>(std::forward<ConsumerStrategy>(strategy));
    }


}
