//
// Created by Michael Repplinger on 20.06.26.
//

#pragma once
#include <vector>

#include "flowgraph/ExecutableGraph.hpp"

namespace clegmed::core {
    template <typename>
    struct IsExecutableGraph : std::false_type {};

    template <typename... Filters>
    struct IsExecutableGraph<ExecutableGraph<Filters...>> : std::true_type {};

    template <typename T>
    concept AnExecutableGraph = IsExecutableGraph<std::decay_t<T>>::value;

    template <typename T, typename... Args>
    concept NotSelfClass = (!std::is_same_v<std::decay_t<Args>,T> && ...);

    template<typename ... ExecutableGraph>
    class CLegMed final {
        std::tuple<ExecutableGraph ...> m_executable_graphs;

    public:
        template<typename ... Args>
        requires NotSelfClass<CLegMed, Args ...> && (AnExecutableGraph<Args>&& ...)
        explicit CLegMed(Args && ...  graphs)
        : m_executable_graphs(std::forward<Args>(graphs) ...) { }

        ~CLegMed() {
            stop();
        }

        void start() {
            auto execute_start = [this]<size_t... Is>(std::index_sequence<Is...>) {
                (std::get<Is>(m_executable_graphs).start(), ...);
            };

            constexpr auto indices = std::make_index_sequence<sizeof...(ExecutableGraph)>{};
            execute_start(indices);
        }

        void stop() {
            auto execute_stop = [this]<size_t... Is>(std::index_sequence<Is...>) {
                (std::get<Is>(m_executable_graphs).stop(), ...);
            };

            constexpr auto indices = std::make_index_sequence<sizeof...(ExecutableGraph)>{};
            execute_stop(indices);
        }
    };


    template <typename... Args>
       CLegMed(Args...) -> CLegMed<std::decay_t<Args>...>;

}
