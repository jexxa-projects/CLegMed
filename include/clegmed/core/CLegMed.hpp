#pragma once
#include <csignal>
#include <filesystem>
#include <vector>

#include "clegmed/plugins/generic/GenericConsumer.hpp"
#include "flowgraph/ExecutableGraph.hpp"
#include "../utils/Signal.hpp"
#include "detail/SignalHandler.hpp"
#include "VersionInfo.hpp"

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
        std::vector<std::string> m_argv;
    public:
        template<typename ... Args>
        requires
            NotSelfClass<CLegMed, Args ...> &&
            (AnExecutableGraph<Args>&& ...) &&
            (sizeof...(Args) == sizeof...(ExecutableGraph))
        explicit CLegMed(Args && ...  graphs)
        : CLegMed(0, nullptr, std::forward<Args>(graphs)...) { }

        template<typename ... Args>
        requires
            (sizeof...(Args) == sizeof...(ExecutableGraph)) &&
            (AnExecutableGraph<Args> && ...)
        explicit CLegMed(const int argc, char* argv[], Args && ...  graphs)
        : m_executable_graphs(std::forward<Args>(graphs) ...) {
            if (argc > 0 && argv != nullptr) {
                m_argv.assign(argv, argv+argc);
            }
        }

        ~CLegMed() {
            stop();
        }

        [[nodiscard]]
        std::string applicationName() const noexcept {
            if (CLEGMED_APP_NAME != std::string("Unknown Application")) {
                return CLEGMED_APP_NAME;
            }

            if (!m_argv.empty()) {
                return std::filesystem::path(std::string(m_argv[0]))
                    .filename()
                    .string();
            }
            return "Unknown Application";
        }

        void start() {
            printStartupInfo();
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
            registerSignalHandler();

            start();
            utils::Logger::log(
                utils::LogLevel::INFO,
                "Application {} successfully started", applicationName()
                );

            detail::wait_for_signal();

            utils::Logger::log(
                utils::LogLevel::INFO,
                "Signal {} received -> Stop the application ", utils::safe_signal_string(detail::get_received_signal()));

            stop();

            utils::Logger::log(
                utils::LogLevel::INFO,
                "Application {} successfully stopped ", applicationName());

        }

        [[nodiscard]]
        VersionInfo versionInfo() const noexcept {
            return VersionInfo(applicationName());
        }

    private:
        void printStartupInfo() {
            utils::Logger::log( utils::LogLevel::INFO, "CLegMed Version              : {} ", versionInfo().m_clegmed_version);
            utils::Logger::log( utils::LogLevel::INFO, "Application Version          : {} ", versionInfo().m_application_version);
            utils::Logger::log( utils::LogLevel::INFO, "Application Name             : {} ", versionInfo().m_application_name);
            utils::Logger::log( utils::LogLevel::INFO, "");
        }
        static void registerSignalHandler() {
            std::signal(SIGINT, handle_shutdown_signals);
            std::signal(SIGHUP, handle_shutdown_signals);
            std::signal(SIGTERM, handle_shutdown_signals);
        }
    };


    template <typename... Args>
       CLegMed(Args...) -> CLegMed<std::decay_t<Args>...>;
    template <typename... Args>
    CLegMed(int, char*[], Args&&...) -> CLegMed<std::decay_t<Args>...>;
}
