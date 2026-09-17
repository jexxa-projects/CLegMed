#pragma once

#include "Filter.hpp"
#include "OutputPipe.hpp"
#include "Traits.hpp"
#include <type_traits>
#include <utility>


namespace clegmed::core {
    template <typename Strategy, typename OutputData, typename FilterProperties>
    concept ValidProducerStrategy =
        requires(Strategy&& strategy) { { strategy() } -> std::same_as<OutputData>; } ||
        requires(Strategy&& strategy, OutputPipe<OutputData>& pipe) { { strategy(pipe) } -> std::same_as<void>; } ||

        (utils::DeserializableFromProperties<FilterProperties> && requires(Strategy&& strategy, FilterProperties& filter_properties)
        { { strategy(filter_properties) } -> std::same_as<OutputData>; }) ||

        (utils::DeserializableFromProperties<FilterProperties> && requires(Strategy&& strategy, OutputPipe<OutputData>& pipe, FilterProperties& filter_properties)
        { { strategy(pipe, filter_properties) } -> std::same_as<void>; }) ;

    template <typename OutputData, typename  ProducerStrategy, typename FilterProperties = std::monostate>
        requires ValidProducerStrategy<ProducerStrategy, OutputData, FilterProperties>
    class Producer : public Filter {
        FilterProperties m_properties;
    public:
        Producer() = delete;
        ~Producer() override = default;

        explicit Producer(ProducerStrategy strategy) : Filter(), m_strategy(std::move(strategy)) {}

        template<typename Self >
        auto&& outputPipe(this Self&& explicit_this) {
            return std::forward<Self>(explicit_this).m_outputPipe;
        }

        void properties(const utils::Properties& properties) {
            if constexpr (!std::is_same_v<FilterProperties, std::monostate>) {
                if (hasProperties()) {
                    m_properties = properties.get<FilterProperties>(propertiesName());
                }
            }
        }

        void produce() {
            // 1. Piped-Signatur (1:n) (Strategy writes directly to the output-pipe)
            if constexpr (std::is_invocable_v<ProducerStrategy, OutputPipe<OutputData>&>) {
                m_strategy(m_outputPipe);
            }
            // 2. 1:1-Signatur (Strategy returns a value that fits to OutputData)
            else if constexpr (std::is_invocable_r_v<OutputData, ProducerStrategy>) {
                m_outputPipe.forward(std::forward<ProducerStrategy>(m_strategy)());
            }
            else if constexpr (std::is_invocable_v<ProducerStrategy, OutputPipe<OutputData>&, const FilterProperties&>) {
                m_strategy(m_outputPipe, m_properties);
            }
            else if constexpr (std::is_invocable_v<ProducerStrategy, const FilterProperties&>) {
                m_outputPipe.forward(m_strategy(m_properties));
            }

            else {
                static_assert(false,
                    "❌ ARCHITECTURE-ERROR: Given ProducerStrategy neither uses "
                    "Piped-Signature (Pipe&) nor 1:1-signature ().");
            }
        }

    private:
        [[no_unique_address]] ProducerStrategy m_strategy;
        OutputPipe<OutputData> m_outputPipe = OutputPipe<OutputData>(*this);
    };

    template <typename ProducerStrategy>
    [[nodiscard]] auto make_producer(ProducerStrategy&& producer_strategy) {
        using OutputData = std::invoke_result_t<ProducerStrategy>;
        using ConcreteProducer = Producer<OutputData, std::decay_t<ProducerStrategy>>;
        return std::make_unique<ConcreteProducer>(std::forward<ProducerStrategy>(producer_strategy));
    }

    template <typename OutputData, typename ProducerStrategy>
    [[nodiscard]] auto make_piped_producer(ProducerStrategy&& producer_strategy) {
        using ConcreteProducer = Producer<OutputData, std::decay_t<ProducerStrategy>>;
        return std::make_unique<ConcreteProducer>(std::forward<ProducerStrategy>(producer_strategy));
    }

    template <typename ProducerStrategy>
    [[nodiscard]] auto make_configured_producer(ProducerStrategy&& producer_strategy) {
        using DecayedStrategy = std::decay_t<ProducerStrategy>;
        using MemberPtr = decltype(&DecayedStrategy::operator());

        using OutputData = detail::function_traits<MemberPtr>::result_type;
        using FilterProperties = detail::function_traits<MemberPtr>::template argument_t<0>;

        using ConcreteProducer = Producer<OutputData, DecayedStrategy, FilterProperties>;
        return std::make_unique<ConcreteProducer>(std::forward<ProducerStrategy>(producer_strategy));
    }

}
