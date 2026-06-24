
#include <semaphore>

#include "clegmed/core/CLegMed.hpp"
#include "clegmed/core/flowgraph/FlowGraph.hpp"
#include "clegmed/plugins/shortcuts.hpp"
#include "clegmed/plugins/generic/GenericConsumer.hpp"
#include "clegmed/plugins/generic/GenericProcessor.hpp"
#include "clegmed/plugins/generic/GenericProducer.hpp"


int main() {
    using namespace clegmed::shortcuts;

    auto flowgraph = FlowGraph{}
        .every(2s)
        .from(emit("Hello World"))
        .then(passThrough())
        .consumeWith(logInfo());

    auto clegmed = CLegMed(std::move(flowgraph));
    clegmed.run();
}

