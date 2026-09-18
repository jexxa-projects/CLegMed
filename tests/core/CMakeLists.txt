# ==============================================================================
# 7. EXECUTABLES / TEST TARGETS
# ==============================================================================

add_executable(clegmed_core_test
        FilterTest.cpp
        ProcessorTest.cpp
        ProducerTest.cpp
        ConsumerTest.cpp
        FlowGraphTest.cpp
        CLegMedTest.cpp
)

add_custom_command(
        TARGET clegmed_core_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/filterproperties.toml"
        "${CMAKE_CURRENT_BINARY_DIR}/filterproperties.toml"
        COMMENT "Kopiere Test-Konfiguration in den Build-Ordner..."
)

target_link_libraries(clegmed_core_test PRIVATE
        clegmed::utils
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
        clegmed::utils
        clegmed::core
        benchmark::benchmark
)

# WICHTIG: Erzwinge maximale Optimierung (-O3) exklusiv für das Benchmark-Target
target_compile_options(clegmed_benchmarks PRIVATE -O3)
