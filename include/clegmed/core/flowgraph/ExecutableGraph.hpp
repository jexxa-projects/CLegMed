//
// Created by Michael Repplinger on 14.06.26.
//

#pragma once
#include <condition_variable>
#include <utility>
#include <tuple>
#include <thread>
#include "FlowGraphConfig.hpp"


namespace clegmed::core {

    template <typename ... Filters>
    class ExecutableGraph {
        std::tuple<Filters...> m_pipeline;
        FlowGraphConfig m_config;
        std::unique_ptr<std::jthread> m_thread_ptr;
        std::condition_variable_any m_cv;
        std::mutex m_cv_mtx;

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

        ~ExecutableGraph() {
            stop();
        }


        void start() {
            switch (m_config.type) {
                case TriggerType::Every:
                    m_thread_ptr =  std::make_unique<std::jthread>([this](std::stop_token stoken) {
                        this->startEvery(stoken);
                });
                    break;

                case TriggerType::Repeat:
                    break;
                case TriggerType::Await:
                    break;
            }
            std::get<0>(m_pipeline).produce();
        }

        void stop() {
            if (m_thread_ptr) {
                m_thread_ptr->request_stop();

            }
            {
                std::unique_lock lock(m_cv_mtx);
                m_cv.notify_all();
            }

            if (m_thread_ptr->joinable()) {
                m_thread_ptr->join();
            }
        }


    private:
        void startEvery(std::stop_token stoken) {
            while (!stoken.stop_requested()) {
                std::get<0>(m_pipeline).produce();

                if (stoken.stop_requested()) {
                    break;
                }

                std::unique_lock lock(m_cv_mtx);
                m_cv.wait_for(lock, stoken, std::chrono::milliseconds(m_config.interval), [] {
                    return false;
                });
            }
        }

    };
    template <typename... Args>
    ExecutableGraph(std::tuple<Args...>, FlowGraphConfig) -> ExecutableGraph<Args...>;

}
