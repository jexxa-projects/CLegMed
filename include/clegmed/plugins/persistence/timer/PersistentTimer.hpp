#pragma once

#include "TimerUtils.hpp"
#include "clegmed/core/Producer.hpp"
#include "clegmed/plugins/persistence/repository/Repository.hpp"

namespace clegmed::plugins::persistence {
    template <typename Repository>
        requires isRepository<Repository, TimerState>
    class PersistentTimer : public core::Producer< TimeInterval,  std::function<void(core::OutputPipe<TimeInterval>&)>>
    {
        using time_point_t = std::chrono::system_clock::time_point;

        const TimerConfig m_timer_config;
        Repository& m_repository;

    public:
        explicit PersistentTimer(const TimerConfig&  timer_config, Repository& repository) :
            Producer([this](core::OutputPipe<TimeInterval>& output_pipe){this->nextTimeInterval(output_pipe);}),
            m_timer_config(timer_config), m_repository(repository)
        {
            if (!m_repository.get(m_timer_config.timer_id).has_value()) {
                m_repository.add(TimerState(timer_config));
            }
        }

        void nextTimeInterval(core::OutputPipe<TimeInterval>& output_pipe) {
            if (auto timer_state = m_repository.get(m_timer_config.timer_id); timer_state.has_value()) {
                const time_point_t now = std::chrono::system_clock::now();

                output_pipe.forward(TimeInterval{.begin = timer_state->last_time_interval.end, .end = now});

                timer_state->last_time_interval.begin = timer_state->last_time_interval.end;
                timer_state->last_time_interval.end = now;

                m_repository.update(timer_state.value());
            } else {
                utils::Logger::log(utils::LogLevel::ERROR, "TimerState is missing");
            }

        }

    };

}
