#pragma once
#include <memory>
#include "Filter.hpp"
#include "Traits.hpp"

namespace clegmed::core {
    template<typename Strategy, typename InputData, typename FilterProperties>
    concept ValidConsumerStrategy =
        requires(Strategy&& strategy, InputData&& input_data)
        { { strategy(std::forward<InputData>(input_data)) } -> std::same_as<void>; } ||

        (utils::DeserializableFromProperties<FilterProperties> &&
        requires(Strategy&& strategy, InputData&& input_data, const FilterProperties& filter_properties)
        {{strategy(std::forward<InputData>(input_data),filter_properties) } -> std::same_as<void>; });


    template <typename InputData, typename ConsumerStrategy, typename FilterProperties = std::monostate>
        requires ValidConsumerStrategy<ConsumerStrategy, InputData, FilterProperties>
    class Consumer : public Filter {
        FilterProperties m_properties;
    public:
        Consumer() = delete;
        explicit Consumer(ConsumerStrategy strategy) : m_strategy(strategy) {}
        ~Consumer() override = default;
        Consumer(Consumer&&) noexcept = default;
        Consumer& operator=(Consumer&&) noexcept = default;

        void properties(const utils::Properties& properties) {
            if constexpr (!std::is_same_v<FilterProperties, std::monostate>) {
                if (hasProperties()) {
                    m_properties = properties.get<FilterProperties>(propertiesName());
                }
            }
        }


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

        using ConcreteConsumer = Consumer<InputData, DecayedStrategy>;

        return std::make_unique<ConcreteConsumer>(
            std::forward<ConsumerStrategy>(strategy)
        );
    }

}
