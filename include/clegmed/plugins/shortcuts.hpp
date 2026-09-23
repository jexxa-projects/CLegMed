#pragma once
#include "../core/CLegMed.hpp"
#include "../core/flowgraph/FlowGraph.hpp"
#include "clegmed/utils/Properties.hpp"
#include "generic/GenericProcessor.hpp"
#include "generic/GenericProducer.hpp"
#include "persistence/generic/PersistentConsumer.hpp"
#include "persistence/timer/PersistentTimer.hpp"

namespace clegmed::shortcuts {
    //Utility classes
    using utils::Properties;

    // Core factory methods
    using core::make_producer;
    using core::make_configured_producer;
    using core::make_configured_pipe_producer;

    using core::make_processor;
    using core::make_configured_processor;
    using core::make_piped_processor;
    using core::make_configured_piped_processor;

    using core::make_consumer;
    using core::make_configured_consumer;

    // Core classes
    using core::OutputPipe;
    using core::FlowGraph;
    using core::CLegMed;

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

    using namespace std::string_literals;
    using namespace std::chrono_literals;
}
