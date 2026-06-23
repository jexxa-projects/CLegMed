#pragma once
#include "ExecutableGraph.hpp"
#include "FlowGraphConfig.hpp"
#include "clegmed/core/Producer.hpp"
#include "clegmed/core/Processor.hpp"
#include "clegmed/core/Consumer.hpp"

namespace clegmed::core {
    template <typename Derived>
    inline constexpr bool is_filter_v = std::is_base_of_v<Filter, Derived>;

    template <typename T>
    struct filter_traits;

    template <typename Input, typename Output, typename Strategy>
    struct filter_traits<Processor<Input, Output, Strategy>> {
        using OutputData = Output;
    };
    template <typename Output, typename Strategy>
    struct filter_traits<Producer<Output, Strategy>> {
        using OutputData = Output;
    };


    template<typename... Filters>
    class PipelineBuilder {
        FlowGraphConfig m_config;
        std::tuple<Filters...> m_pipeline;

        template<typename... OtherFilters>
        friend class PipelineBuilder;

        explicit PipelineBuilder(std::tuple<Filters...> &&pipeline, const FlowGraphConfig& config)
            : m_config(config), m_pipeline(std::move(pipeline)) {}

    public:
        explicit PipelineBuilder(const FlowGraphConfig &config)
            : m_config(config), m_pipeline{} {
        }

        template<typename Producer>
        auto from(Producer &&producer) && {
            using DecayedProducer = std::decay_t<Producer>;

            if constexpr (is_filter_v<DecayedProducer>) {
                return PipelineBuilder<DecayedProducer>(
                    std::make_tuple(std::forward<Producer>(producer)), m_config
                );
            } else {
                return std::move(*this).from(make_producer(std::forward<Producer>(producer)));
            }
        }

        template<typename Processor>
        auto then(Processor &&processor) && {
            using DecayedProcessor = std::decay_t<Processor>;
            using LastFilter = std::tuple_element_t<sizeof...(Filters) - 1, std::tuple<Filters...>>;
            using PreviousOutputType = filter_traits<LastFilter>::OutputData;

            if constexpr (requires { { processor.template build<PreviousOutputType>() }; })
            {
                auto concrete_processor = std::forward<Processor>(processor).template build<PreviousOutputType>();
                return std::move(*this).then(std::move(concrete_processor));
            } else if constexpr (
                requires { typename DecayedProcessor::PassThroughFactory; } ||
                requires { { processor.template build<PreviousOutputType>() }; })
            {
                auto concrete_processor = std::forward<Processor>(processor).template build<PreviousOutputType>();
                return std::move(*this).then(std::move(concrete_processor));
            } else if constexpr (is_filter_v<DecayedProcessor>) {
                auto new_tuple = std::tuple_cat(
                    std::move(m_pipeline),
                    std::tuple<DecayedProcessor>{std::forward<Processor>(processor)}
                );

                return PipelineBuilder<Filters..., DecayedProcessor>(
                    std::move(new_tuple),
                    m_config
                );
            } else {
                return std::move(*this).then(make_processor(std::forward<Processor>(processor)));
            }

        }

        template<typename Consumer>
        auto consumeWith(Consumer &&consumer) && {
            using DecayedConsumer = std::decay_t<Consumer>;

            if constexpr (is_filter_v<DecayedConsumer>) {
                auto new_tuple = std::tuple_cat(
                    std::move(m_pipeline),
                    std::tuple<DecayedConsumer>(std::forward<Consumer>(consumer))
                );

                return ExecutableGraph(std::move(new_tuple), m_config);
            } else {
                return std::move(*this).consumeWith(make_consumer(std::forward<Consumer>(consumer)));
            }
        }
    };
}
