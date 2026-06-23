#pragma once


#include "FlowGraphConfig.hpp"
#include "PipelineBuilder.hpp"

namespace clegmed::core {
    class FlowGraph {
        FlowGraphConfig m_config;

    public:
        FlowGraph() = default;

        auto every(const std::chrono::milliseconds interval) && {
            m_config.type = TriggerType::Every;
            m_config.interval = interval;
            return PipelineBuilder(m_config);
        }

        auto repeat(const size_t count) && {
            m_config.type = TriggerType::Repeat;
            m_config.repeat_count = count;
            return PipelineBuilder(m_config);
        }

        auto await() && {
            m_config.type = TriggerType::Await;
            return PipelineBuilder(m_config);
        }
    };


}


