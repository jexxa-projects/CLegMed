#pragma once
#include <chrono>

namespace clegmed::core {
    enum class TriggerType { Every, Repeat, Await };
    struct FlowGraphConfig {
        TriggerType type{TriggerType::Every};
        std::chrono::milliseconds interval{0};
        size_t repeat_count{0};
    };
}