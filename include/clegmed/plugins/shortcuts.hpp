#pragma once
#include "../core/CLegMed.hpp"
#include "../core/flowgraph/FlowGraph.hpp"
#include "generic/GenericProcessor.hpp"
#include "generic/GenericProducer.hpp"

namespace clegmed::shortcuts {
    // Core classes
    using core::FlowGraph;
    using core::CLegMed;
    using namespace std::chrono_literals;

    // Plugins producer
    using plugins::generic::emit;

    // Plugins processor
    using plugins::generic::passThrough;
    using plugins::generic::traceInfo;
    using plugins::generic::traceWarn;
    using plugins::generic::traceError;
    using plugins::generic::append;

    // Plugins consumer
    using plugins::generic::logInfo;
    using plugins::generic::logWarn;
    using plugins::generic::logError;
    using plugins::generic::discard;
    using plugins::generic::store;


}
