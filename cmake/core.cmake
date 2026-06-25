add_library(clegmed_core_base INTERFACE)
add_library(clegmed::core::base ALIAS clegmed_core_base)

target_sources(clegmed_core_base INTERFACE
        $<BUILD_INTERFACE:
             ${CMAKE_SOURCE_DIR}/include/clegmed/core/Filter.hpp
             ${CMAKE_SOURCE_DIR}/include/clegmed/core/InputPipe.hpp
             ${CMAKE_SOURCE_DIR}/include/clegmed/core/Processor.hpp
             ${CMAKE_SOURCE_DIR}/include/clegmed/core/OutputPipe.hpp
             ${CMAKE_SOURCE_DIR}/include/clegmed/core/Producer.hpp
             ${CMAKE_SOURCE_DIR}/include/clegmed/core/Consumer.hpp
             ${CMAKE_SOURCE_DIR}/include/clegmed/core/Traits.hpp
             ${CMAKE_SOURCE_DIR}/include/clegmed/core/Pipe.hpp
             ${CMAKE_SOURCE_DIR}/include/clegmed/core/CLegMed.hpp
             ${CMAKE_SOURCE_DIR}/include/clegmed/core/flowgraph/FlowGraph.hpp
             ${CMAKE_SOURCE_DIR}/include/clegmed/core/flowgraph/FlowGraphConfig.hpp
             ${CMAKE_SOURCE_DIR}/include/clegmed/core/flowgraph/ExecutableGraph.hpp
        >
)

target_include_directories(clegmed_core_base INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

