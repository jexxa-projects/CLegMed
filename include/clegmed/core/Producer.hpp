#pragma once

#include "Filter.hpp"
#include "OutputPipe.hpp"
#include "Traits.hpp"
#include "PipelineError.hpp"
#include "concepts/ProducerStrategy.hpp"
#include <type_traits>
#include <utility>


namespace clegmed::core {

    template <
        typename OutputData,
        typename Strategy,
        typename FilterProperties = std::monostate>

    requires ValidProducerStrategy<Strategy, OutputData, FilterProperties>
    class Producer : public Filter {
    public:
        Producer() = delete;
        ~Producer() override = default;

        explicit Producer(Strategy strategy) : Filter(), m_strategy(std::move(strategy)) {}

        template<typename Self >
        auto&& outputPipe(this Self&& explicit_this) {
            return std::forward<Self>(explicit_this).m_output_pipe;
        }

        template<typename Self >
        auto&& errorPipe(this Self&& explicit_this) {
            return std::forward<Self>(explicit_this).m_error_pipe;
        }

        void properties(const utils::Properties& properties) {
            if constexpr (!std::is_same_v<FilterProperties, std::monostate>) {
                if (hasProperties()) {
                    m_properties = properties.get<FilterProperties>(propertiesName());
                }
            }
        }

        void produce() noexcept{
            try {
                executeStrategy();
            } catch(...) {
                emitOnError(std::current_exception());
            }
        }

    private:
        using PipelineResult = std::expected<void, PipelineError<OutputData>>;

        void executeStrategy()
        {
            if constexpr (ProducerWithPipe<Strategy, OutputData>) {
                m_strategy(m_output_pipe);
            }
            else if constexpr ( ProducerWithDirectReturn<Strategy, OutputData> ) {
                return m_output_pipe.forward( std::move(m_strategy()) );
            }
            else if constexpr (ProducerWithPropertiesAndPipe<Strategy, OutputData, FilterProperties>) {
                m_strategy(m_output_pipe, m_properties);
            }
            else if constexpr (ProducerWithPropertiesAndReturn<Strategy, OutputData, FilterProperties>) {
                 m_output_pipe.forward( std::move(m_strategy(m_properties)) );
            }
            else {
                static_assert(false, "❌ ARCHITECTURE-ERROR: Unsupported ProducerStrategy signature");
            }
        }

        void emitOnError(const std::exception_ptr& exception) {
            if (!exception) {
                return;
            }

            try {
                std::rethrow_exception(exception);
            }
            catch (const std::exception& e) {
                utils::Logger::log(utils::LogLevel::ERROR,
                    "Pipeline Error: error occurred. {}" , e.what()
                    );
                if (m_error_pipe.isConnected()) {
                    m_error_pipe.forward(PipelineError<OutputData>(ProcessingError(name(), e.what())));
                } else {
                    utils::Logger::log(utils::LogLevel::ERROR,
                        "Pipeline Error: error occurred and no error pipe is connected. " , e.what()
                        );
                }
            }
            catch (...) {
                utils::Logger::log(utils::LogLevel::ERROR,
                    "Pipeline Error: Unknown error occurred .");
            }
        }

        OutputPipe<OutputData> m_output_pipe = OutputPipe<OutputData>(*this);
        OutputPipe<PipelineError<OutputData>> m_error_pipe = OutputPipe<PipelineError<OutputData>>(*this);
        [[no_unique_address]] Strategy m_strategy;
        [[no_unique_address]] FilterProperties m_properties;
    };

    template <typename ProducerStrategy>
    [[nodiscard]] auto makeProducer(ProducerStrategy&& producer_strategy) {
        using OutputData = std::invoke_result_t<ProducerStrategy>;
        using ConcreteProducer = Producer<OutputData, std::decay_t<ProducerStrategy>>;
        return std::make_unique<ConcreteProducer>(std::forward<ProducerStrategy>(producer_strategy));
    }

    template <typename OutputData, typename ProducerStrategy>
    [[nodiscard]] auto makePipedProducer(ProducerStrategy&& producer_strategy) {
        using ConcreteProducer = Producer<OutputData, std::decay_t<ProducerStrategy>>;
        return std::make_unique<ConcreteProducer>(std::forward<ProducerStrategy>(producer_strategy));
    }

    template <typename ProducerStrategy>
    [[nodiscard]] auto makeConfiguredProducer(ProducerStrategy&& producer_strategy) {
        using DecayedStrategy = std::decay_t<ProducerStrategy>;
        using MemberPtr = decltype(&DecayedStrategy::operator());

        using OutputData = detail::function_traits<MemberPtr>::result_type;
        using FilterProperties = detail::function_traits<MemberPtr>::template argument_t<0>;

        using ConcreteProducer = Producer<OutputData, DecayedStrategy, FilterProperties>;
        return std::make_unique<ConcreteProducer>(std::forward<ProducerStrategy>(producer_strategy));
    }

    template <typename ProducerStrategy>
    [[nodiscard]] auto makeConfiguredPipeProducer(ProducerStrategy&& producer_strategy) {
        using DecayedStrategy = std::decay_t<ProducerStrategy>;
        using MemberPtr = decltype(&DecayedStrategy::operator());

        using PipeArg   = detail::function_traits<MemberPtr>::template argument_t<0>;
        using FilterProperties = detail::function_traits<MemberPtr>::template argument_t<1>;
        using OutputData = detail::extract_pipe_type_t<PipeArg>;

        using ConcreteProducer = Producer<OutputData, DecayedStrategy, FilterProperties>;

        return std::make_unique<ConcreteProducer>(
            std::forward<ProducerStrategy>(producer_strategy)
        );
    }

}
