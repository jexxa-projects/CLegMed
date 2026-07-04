
// ReSharper disable CppParameterMayBeConst
#include "clegmed/plugins/shortcuts.hpp"


int main(int argc, char** argv) {
    using namespace clegmed::shortcuts;

    auto clegmed = CLegMed(argc, argv,
        FlowGraph{}
            .every(2s)
            .from(emit("Hello "))
            .then(append("World"))
            .consumeWith(logInfo())
    );

    clegmed.run();
}
