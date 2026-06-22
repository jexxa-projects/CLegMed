//
// Created by Michael Repplinger on 20.06.26.
//

#pragma once
#include <csignal>
#include <semaphore>
#include <vector>

#include "clegmed/plugins/generic/GenericConsumer.hpp"
#include "flowgraph/ExecutableGraph.hpp"

namespace clegmed::core {
    inline std::binary_semaphore g_signal_semaphore{0};
    inline std::atomic g_received_signal{0};

    // Signal handling
    extern "C" inline void handle_shutdown_signals(const int signal) {
        if (signal == SIGINT || signal == SIGTERM || signal == SIGHUP) {
            g_received_signal = signal;
            g_signal_semaphore.release();
        }
    }

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

        void run() {
            start();
            registerSignalHandler();
            g_signal_semaphore.acquire();
            utils::Logger::log(utils::LogLevel::INFO, "Signal {} received -> Stop the application", strsignal(g_received_signal.load()));

            stop();
        }

    private:
        static void registerSignalHandler() {
            std::signal(SIGINT, handle_shutdown_signals);
            std::signal(SIGHUP, handle_shutdown_signals);
            std::signal(SIGTERM, handle_shutdown_signals);
        }
    };


    template <typename... Args>
       CLegMed(Args...) -> CLegMed<std::decay_t<Args>...>;

}
