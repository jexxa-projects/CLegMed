add_library(clegmed_core_base INTERFACE)
add_library(clegmed::core::base ALIAS clegmed_core_base)

target_sources(clegmed_core_base INTERFACE
        FILE_SET HEADERS
        BASE_DIRS include
        FILES
        include/clegmed/core/Filter.hpp
        include/clegmed/core/InputPipe.hpp
        include/clegmed/core/Processor.hpp
        include/clegmed/core/OutputPipe.hpp
        include/clegmed/core/Producer.hpp
        include/clegmed/core/Consumer.hpp
        include/clegmed/core/Traits.hpp
        include/clegmed/core/Pipe.hpp
        include/clegmed/core/CLegMed.hpp
        include/clegmed/core/flowgraph/FlowGraph.hpp
        include/clegmed/core/flowgraph/FlowGraphConfig.hpp
        include/clegmed/core/flowgraph/ExecutableGraph.hpp
)