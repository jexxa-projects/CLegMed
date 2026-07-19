#pragma once
#include <chrono>

namespace clegmed::plugins::persistence {
    struct TimerId {
        std::string timer_id;
        auto operator<=>(const TimerId&) const = default;
    };

    struct TimeInterval {
        std::chrono::system_clock::time_point begin;
        std::chrono::system_clock::time_point end;
        auto operator<=>(const TimeInterval&) const = default;

        [[nodiscard]] static TimeInterval of(
            std::chrono::system_clock::time_point begin,
            std::chrono::system_clock::time_point end) {
            return TimeInterval{begin, end};
        }

    };

}
