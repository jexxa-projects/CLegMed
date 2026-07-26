#pragma once
#include "../core/CLegMed.hpp"
#include "../core/flowgraph/FlowGraph.hpp"
#include "generic/GenericProcessor.hpp"
#include "generic/GenericProducer.hpp"
#include "persistence/generic/PersistentConsumer.hpp"
#include "persistence/timer/PersistentTimer.hpp"

namespace clegmed::shortcuts {
    // Core classes
    using core::FlowGraph;
    using core::CLegMed;
    using namespace std::chrono_literals;

    // Plugins producer
    using plugins::generic::emit;
    using plugins::persistence::persistentTimer;

    // Plugins processor
    using plugins::generic::passThrough;
    using plugins::generic::traceInfo;
    using plugins::generic::traceWarn;
    using plugins::generic::traceError;
    using plugins::generic::append;
    using plugins::persistence::startWithLookBack;

    // Plugins consumer
    using plugins::generic::logInfo;
    using plugins::generic::logWarn;
    using plugins::generic::logError;
    using plugins::generic::discard;
    using plugins::generic::store;
    using plugins::persistence::persistentStore;


}
