#pragma once
#include "ExecutableGraph.hpp"
#include "FlowGraphConfig.hpp"
#include "clegmed/core/Producer.hpp"
#include "clegmed/core/Processor.hpp"
#include "clegmed/core/Consumer.hpp"
#include <memory>
#include <tuple>
#include <type_traits>

namespace clegmed::core {

    template <typename Derived>
    inline constexpr bool is_filter_v = std::is_base_of_v<Filter, Derived>;
    // Hilfs-Trait um zu prüfen, ob ein Typ ein std::unique_ptr ist
    template <typename >
    struct is_unique_ptr : std::false_type {};
    template <typename T>
    struct is_unique_ptr<std::unique_ptr<T>> : std::true_type {};
    template <typename T>
    inline constexpr bool is_unique_ptr_v = is_unique_ptr<T>::value;

    // Hilfs-Trait um zu prüfen, ob ein Typ ein std::unique_ptr ist
    template <typename >
    struct extract_ptr_element { using type = void; };

    template <typename T>
    struct extract_ptr_element<std::unique_ptr<T>> { using type = T; };

    template <typename T>
    using extract_ptr_element_t = extract_ptr_element<T>::type;
    // C++23 optimierte Typ-Extraktion für den Output
    template <typename T>
    struct extract_output_type {
        using type = decltype([](T*) {
            if constexpr (requires { []<typename Out, typename Strat>(const Producer<Out, Strat>*)
                {/* Empty because we need the lambda just for inheritance validation*/}
                (static_cast<T*>(nullptr)); }) {
                return []<typename Out, typename Strat>(const Producer<Out, Strat>*) {
                    return std::type_identity<Out>{};
                }(static_cast<T*>(nullptr));
            } else {
                return []<typename In, typename Out, typename Strat>(const Processor<In, Out, Strat>*) {
                    return std::type_identity<Out>{};
                }(static_cast<T*>(nullptr));
            }
        }(static_cast<T*>(nullptr)))::type;
    };
    template <typename T>
    using extract_output_type_t = extract_output_type<T>::type;

    template<typename... Filters>
    class PipelineBuilder {
        FlowGraphConfig m_config;
        [[no_unique_address]] std::tuple<std::unique_ptr<Filters>...> m_pipeline;

        template<typename... OtherFilters>
        friend class PipelineBuilder;

        explicit PipelineBuilder(std::tuple<std::unique_ptr<Filters>...> &&pipeline, const FlowGraphConfig& config)
            : m_config(config), m_pipeline(std::move(pipeline)) {}

    public:
        explicit PipelineBuilder(const FlowGraphConfig &config)
            : m_config(config), m_pipeline{} {}

              // --- FROM ---
        template<typename ProducerArg>
        auto from(ProducerArg &&producer) && {
            using DecayedArg = std::decay_t<ProducerArg>;

            // 1. Fall: Es ist bereits ein unique_ptr
            if constexpr (is_unique_ptr_v<DecayedArg>) {
                using ActualFilter = extract_ptr_element_t<DecayedArg>;
                static_assert(is_filter_v<ActualFilter>, "The type inside unique_ptr must inherit from Filter!");

                return PipelineBuilder<ActualFilter>(
                    std::make_tuple(std::forward<ProducerArg>(producer)), m_config
                );
            }
            // 2. Fall: Es ist ein rohes Filter-Objekt (Trifft auf Ihren PersistentTimer zu!)
            else if constexpr (is_filter_v<DecayedArg>) {
                auto ptr = std::make_unique<DecayedArg>(std::forward<ProducerArg>(producer));
                return PipelineBuilder<DecayedArg>(
                    std::make_tuple(std::move(ptr)), m_config
                );
            }
            // 3. Fall: Lambda / Hilfsfunktion
            else {
                return std::move(*this).from(make_producer(std::forward<ProducerArg>(producer)));
            }
        }

        template<typename ProcessorArg>
        auto then(ProcessorArg &&processor) && {
            using DecayedArg = std::decay_t<ProcessorArg>;

            using LastFilter = std::tuple_element_t<sizeof...(Filters) - 1, std::tuple<Filters...>>;
            using PreviousOutputType = extract_output_type_t<LastFilter>;

            // Fall 1: Lambda Ausdrücke
            if constexpr (std::is_invocable_v<DecayedArg, decltype(std::declval<PreviousOutputType>())>) {
                if constexpr (requires { &DecayedArg::operator(); }) {
                    return std::move(*this).then(make_processor(std::forward<ProcessorArg>(processor)));
                } else {
                    return std::move(*this).then(make_processor<PreviousOutputType>(std::forward<ProcessorArg>(processor)));
                }
            }
            // Fall 2: Bereits ein unique_ptr
            else if constexpr (is_unique_ptr_v<DecayedArg>) {
                using ActualFilter = extract_ptr_element_t<DecayedArg>;
                static_assert(is_filter_v<ActualFilter>, "The type inside unique_ptr must inherit from Filter!");

                auto new_tuple = std::tuple_cat(
                    std::move(m_pipeline),
                    std::tuple<std::unique_ptr<ActualFilter>>{std::forward<ProcessorArg>(processor)}
                );
                return PipelineBuilder<Filters..., ActualFilter>(std::move(new_tuple), m_config);
            }
            // Fall 3: Rohes Filter-Objekt
            else if constexpr (is_filter_v<DecayedArg>) {
                auto ptr = std::make_unique<DecayedArg>(std::forward<ProcessorArg>(processor));

                auto new_tuple = std::tuple_cat(
                    std::move(m_pipeline),
                    std::tuple<std::unique_ptr<DecayedArg>>{std::move(ptr)}
                );
                return PipelineBuilder<Filters..., DecayedArg>(std::move(new_tuple), m_config);
            }
            else {
                return std::move(*this).then(make_processor(std::forward<ProcessorArg>(processor)));
            }
        }

        template<typename ConsumerArg>
        auto consumeWith(ConsumerArg &&consumer) && {
            using DecayedArg = std::decay_t<ConsumerArg>;

            if constexpr (is_unique_ptr_v<DecayedArg>) {
                using ActualFilter = extract_ptr_element_t<DecayedArg>;
                static_assert(is_filter_v<ActualFilter>, "The type inside unique_ptr must inherit from Filter!");

                auto new_tuple = std::tuple_cat(
                    std::move(m_pipeline),
                    std::tuple<std::unique_ptr<ActualFilter>>(std::forward<ConsumerArg>(consumer))
                );
                return ExecutableGraph(std::move(new_tuple), m_config);
            }
            else if constexpr (is_filter_v<DecayedArg>) {
                auto ptr = std::make_unique<DecayedArg>(std::forward<ConsumerArg>(consumer));
                auto new_tuple = std::tuple_cat(
                    std::move(m_pipeline),
                    std::tuple<std::unique_ptr<DecayedArg>>(std::move(ptr))
                );
                return ExecutableGraph(std::move(new_tuple), m_config);
            } else {
                return std::move(*this).consumeWith(make_consumer(std::forward<ConsumerArg>(consumer)));
            }
        }

    };
}
