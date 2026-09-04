add_library(clegmed_core_base INTERFACE)
add_library(clegmed::core::base ALIAS clegmed_core_base)

# 1. Datei in den Build-Ordner generieren (Bleibt so)
configure_file(
        include/clegmed/core/Version.hpp.in
        ${CMAKE_CURRENT_BINARY_DIR}/include/clegmed/core/generated/Version.hpp
)

# 2. Beide Include-Pfade für das Interface registrieren (WICHTIG!)
target_include_directories(clegmed_core_base INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
)

# 3. Das File-Set enthält nur noch die echten, statischen Quellcodedateien
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
        include/clegmed/core/detail/SignalHandler.hpp
)
