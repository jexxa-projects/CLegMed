//
// Created by Michael Repplinger on 14.06.26.
//

#pragma once
#include <utility>
#include <tuple>
#include "FlowGraphConfig.hpp"


namespace clegmed::core {

    template <typename ... Filters>
    class ExecutableGraph {
        std::tuple<Filters...> m_pipeline;
        FlowGraphConfig m_config;
        template <size_t... Is>
          void connect_pipes(std::index_sequence<Is...>) {
            ((std::get<Is>(m_pipeline).outputPipe().connect(std::get<Is + 1>(m_pipeline).inputPipe())), ...);
        }

        public:
        ExecutableGraph(std::tuple<Filters...>&& pipeline, const FlowGraphConfig& config)
                : m_pipeline(std::move(pipeline)), m_config(config) {
            constexpr size_t total_elements = sizeof...(Filters);
            static_assert(total_elements >= 2, "Pipeline braucht mindestens Producer und Consumer!");

            connect_pipes(std::make_index_sequence<total_elements - 1>{});
        }


        void run() {
            std::get<0>(m_pipeline).produce();
        }

    };
    template <typename... Args>
    ExecutableGraph(std::tuple<Args...>, FlowGraphConfig) -> ExecutableGraph<Args...>;

}
