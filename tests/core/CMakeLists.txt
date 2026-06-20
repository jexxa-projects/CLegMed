# ==============================================================================
# 7. EXECUTABLES / TEST TARGETS
# ==============================================================================

add_executable(clegmed_core_test
        FilterTest.cpp
        ProcessorTest.cpp
        ProducerTest.cpp
        FlowGraphTest.cpp
        CLegMedTest.cpp
)
target_link_libraries(clegmed_core_test PRIVATE
        clegmed::core
        gtest
        gtest_main
)
gtest_discover_tests(clegmed_core_test)

# --- 7.2 GOOGLE BENCHMARK (Für Performance-Messungen) ---
add_executable(clegmed_benchmarks
        FlowGraphBenchmark.cpp
        ../../include/clegmed/plugins/generic/GenericProcessor.hpp
        ../../include/clegmed/plugins/generic/GenericConsumer.hpp
        ../../include/clegmed/core/Pipe.hpp
        ../../include/clegmed/core/flowgraph/FlowGraph.hpp
        ../../include/clegmed/core/flowgraph/FlowGraphConfig.hpp
        ../../include/clegmed/core/flowgraph/PipelineBuilder.hpp
        ../../include/clegmed/core/flowgraph/ExecutableGraph.hpp
        ../../include/clegmed/core/CLegMed.hpp
        ../utils/Await.hpp
)
target_link_libraries(clegmed_benchmarks PRIVATE
        clegmed::core
        benchmark::benchmark
)

# WICHTIG: Erzwinge maximale Optimierung (-O3) exklusiv für das Benchmark-Target
target_compile_options(clegmed_benchmarks PRIVATE -O3)
