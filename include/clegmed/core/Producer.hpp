#pragma once
#include <iostream>
#include <ostream>

#include "Filter.hpp"
#include "OutputPipe.hpp"
#include <type_traits>
#include <utility>


namespace clegmed::core {
    template <typename Strategy, typename OutputData>
    concept ValidProducerStrategy =
        requires(Strategy&& strategy) { { strategy() } -> std::same_as<OutputData>; } ||
        requires(Strategy&& strategy, OutputPipe<OutputData>& pipe) { { strategy(pipe) } -> std::same_as<void>; };

    template <typename OutputData, typename  ProducerStrategy>
        requires ValidProducerStrategy<ProducerStrategy, OutputData>
    class Producer : public Filter {
    public:
        Producer() = delete;
        ~Producer() override = default;

        explicit Producer(ProducerStrategy strategy) : Filter(), m_strategy(std::move(strategy)) {}
        Producer(Producer&&) noexcept = default;
        Producer& operator=(Producer&&) noexcept = default;

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
            } else {
                static_assert(false,
                    "❌ ARCHITECTURE-ERROR: Given ProducerStrategy neither uses "
                    "Piped-Signature (Pipe&) nor 1:1-signature ().");
            }
        }

    private:
        ProducerStrategy m_strategy;
        OutputPipe<OutputData> m_outputPipe = OutputPipe<OutputData>(*this);
    };

    template <typename  ProducerStrategy>
    [[nodiscard]] auto make_producer(ProducerStrategy&& producer_strategy) {
        using OutputData = std::invoke_result_t<ProducerStrategy>;
        return Producer<OutputData, std::decay_t<ProducerStrategy>>(std::forward<ProducerStrategy>(producer_strategy));
    }

    template <typename OutputData, typename  ProducerStrategy>
    [[nodiscard]] auto make_piped_producer(ProducerStrategy&& producer_strategy) {
        return Producer<OutputData, std::decay_t<ProducerStrategy>>(std::forward<ProducerStrategy>(producer_strategy));
    }


    template <typename T>
    struct is_producer_class : std::false_type {};

    template <typename OutputData, typename ProducerStrategy>
    struct is_producer_class<Producer<OutputData, ProducerStrategy>> : std::true_type {};

    template <typename T>
    inline constexpr bool is_producer_class_v = is_producer_class<std::decay_t<T>>::value;

}
