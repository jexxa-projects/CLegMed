#pragma once
#include <chrono>
#include "clegmed/utils/EntityTraits.hpp"

namespace clegmed::plugins::persistence {
    struct TimerId {
        std::string timer_id;
        auto operator<=>(const TimerId&) const = default;
    };

    struct TimerConfig {
        TimerId timer_id;
        std::chrono::system_clock::time_point start_time;

        auto operator<=>(const TimerConfig&) const = default;

        [[nodiscard]] static TimerConfig timerConfigOf(const TimerId &timer_id)
        {
            return TimerConfig{.timer_id = timer_id, .start_time = std::chrono::system_clock::now()};
        }

        [[nodiscard]] static TimerConfig timerConfigOf(const TimerId& timer_id, const std::chrono::system_clock::time_point start_time)
        {
            return TimerConfig{ .timer_id = timer_id, .start_time = start_time};
        }
    };


    struct TimeInterval {
        std::chrono::system_clock::time_point begin;
        std::chrono::system_clock::time_point end;
        auto operator<=>(const TimeInterval&) const = default;

        [[nodiscard]] static TimeInterval of(
            const std::chrono::system_clock::time_point begin,
            const std::chrono::system_clock::time_point end) {
            return TimeInterval{.begin = begin, .end = end};
        }
    };

    struct TimerState {
        TimerId timer_id;
        TimeInterval last_time_interval;

        explicit TimerState(const TimerConfig& timer_config)
        : timer_id(timer_config.timer_id), last_time_interval(timer_config.start_time, timer_config.start_time) {
        }

        TimerState(const TimerState&) = delete;
        TimerState& operator=(const TimerState&) = delete;

        TimerState(TimerState&&) = default;
        TimerState& operator=(TimerState&&) = default;

        auto operator<=>(const TimerState&) const = default;
    };


};

template <>
struct clegmed::utils::EntityTraits<clegmed::plugins::persistence::TimerState> {
    static auto getId(const plugins::persistence::TimerState& obj) {
        return obj.timer_id;
    }
};

