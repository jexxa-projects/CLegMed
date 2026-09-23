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
    using core::makeProducer;
    using core::makeConfiguredProducer;
    using core::makeConfiguredPipeProducer;

    using core::makeProcessor;
    using core::makeConfiguredProcessor;
    using core::makePipedProcessor;
    using core::makeConfiguredPipedProcessor;

    using core::makeConsumer;
    using core::makeConfiguredConsumer;

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
