
#include <semaphore>

#include "clegmed/core/CLegMed.hpp"
#include "clegmed/core/flowgraph/FlowGraph.hpp"
#include "clegmed/plugins/generic/GenericConsumer.hpp"
#include "clegmed/plugins/generic/GenericProcessor.hpp"
#include "clegmed/plugins/generic/GenericProducer.hpp"


int main() {
    using namespace clegmed::core;
    using namespace clegmed::plugins::generic;
    using std::chrono::seconds;

    auto flowgraph = FlowGraph{}
        .every(seconds(2))
        .from(emit("Hello World"))
        .then(passThrough<std::string>())
        .consumeWith(logInfo<std::string>());

    auto clegmed = CLegMed(std::move(flowgraph));
    clegmed.run();
}

