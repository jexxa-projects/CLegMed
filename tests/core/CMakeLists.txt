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
        ../utils/Await.hpp
        ../../include/clegmed/utils/Logger.hpp
        ../../include/clegmed/utils/Signal.hpp
)
target_link_libraries(clegmed_benchmarks PRIVATE
        clegmed::core
        benchmark::benchmark
)

# WICHTIG: Erzwinge maximale Optimierung (-O3) exklusiv für das Benchmark-Target
target_compile_options(clegmed_benchmarks PRIVATE -O3)
