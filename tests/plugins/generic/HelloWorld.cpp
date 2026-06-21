
#include <semaphore>
#include <csignal>

#include "clegmed/core/CLegMed.hpp"
#include "clegmed/core/flowgraph/FlowGraph.hpp"
#include "clegmed/plugins/generic/GenericConsumer.hpp"
#include "clegmed/plugins/generic/GenericProcessor.hpp"
#include "clegmed/plugins/generic/GenericProducer.hpp"

std::binary_semaphore signal_semaphore{0};

// Signal handling
extern "C" void handle_ctrl_c(const int signal) {
    if (signal == SIGINT) {
        signal_semaphore.release();
    }
}

int main() {
   using namespace clegmed::core;
   using namespace clegmed::plugins::generic;
   using std::chrono::seconds;

   auto flowgraph = FlowGraph{}
        .every(seconds(2))
        .from(emit("Hello World"))
        .then(passThrough<std::string>())
        .consumeWith(logInfo<std::string>());

    flowgraph.start();

    signal_semaphore.acquire();

    flowgraph.stop();
}

