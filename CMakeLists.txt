# ==============================================================================
# 1. PREAMBLE & REQUIREMENTS
# ==============================================================================
cmake_minimum_required(VERSION 4.1)
project(clegmed VERSION 0.1.0 LANGUAGES CXX)

# ==============================================================================
# 2. GLOBAL SETTINGS & COMPILER OPTIONS
# ==============================================================================
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==============================================================================
# 3. EXTERNAL DEPENDENCIES (Placeholders for future use, e.g., Qt)
# ==============================================================================


# ==============================================================================
# 4. TARGET DEFINITION (The core shared library)
# ==============================================================================
add_library(clegmed SHARED
        src/core/Filter.cpp
        include/clegmed/core/Filter.hpp
        include/clegmed/core/InputPipe.hpp
        include/clegmed/core/Processor.hpp
        include/clegmed/core/OutputPipe.hpp
        include/clegmed/core/Producer.hpp
        include/clegmed/core/Consumer.hpp
        include/clegmed/core/Traits.hpp
)

# ==============================================================================
# 5. TARGET PROPERTIES (Metadata and ABI versioning)
# ==============================================================================

set_target_properties(clegmed PROPERTIES
        VERSION 0.1.0
        SOVERSION 0
)


# ==============================================================================
# 6. TARGET RELATIONSHIPS (Include paths and linking)
# ==============================================================================
target_include_directories(clegmed PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)


# ==============================================================================
# 7. EXECUTABLES / TEST TARGETS
# ==============================================================================
# Enable CMake's internal testing infrastructure
include(CTest)
include(GoogleTest)

# Automatically download and include GoogleTest from GitHub
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.17.0 # Secure, reproducible release tag for production
)

# Forbit gtest from overriding our global compiler settings settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(clegmed_core_test
        tests/core/FilterTest.cpp
        tests/core/ProcessorTest.cpp
        tests/core/ProducerTest.cpp
        tests/core/FlowGraphTest.cpp
)
target_link_libraries(clegmed_core_test PRIVATE
        clegmed
        gtest
        gtest_main
)
gtest_discover_tests(clegmed_core_test)

# --- 7.2 GOOGLE BENCHMARK (Für Performance-Messungen) ---
FetchContent_Declare(
        google_benchmark
        GIT_REPOSITORY https://github.com/google/benchmark
        GIT_TAG        v1.9.5
)
# Interne Tests von Google Benchmark deaktivieren, um Build-Zeit zu sparen
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(google_benchmark)

# Separates Target NUR für deine Benchmarks
add_executable(clegmed_benchmarks
        tests/core/FlowGraphBenchmark.cpp
)
target_link_libraries(clegmed_benchmarks PRIVATE
        clegmed
        benchmark::benchmark
)

# WICHTIG: Erzwinge maximale Optimierung (-O3) exklusiv für das Benchmark-Target
target_compile_options(clegmed_benchmarks PRIVATE -O3)
# ==============================================================================
# 8. Add doxygen
# ==============================================================================
find_package(Doxygen REQUIRED)

if(DOXYGEN_FOUND)
    set(DOXYGEN_PROJECT_NAME "${PROJECT_NAME}")
    set(DOXYGEN_PROJECT_NUMBER "${PROJECT_VERSION}")

    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_LATEX NO)
    set(DOXYGEN_RECURSIVE YES)
    set(DOXYGEN_BUILTIN_STL_SUPPORT YES)
    set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/docs")

    doxygen_add_docs(docs
            ${PROJECT_SOURCE_DIR}/src
            ${PROJECT_SOURCE_DIR}/include
            COMMENT "Generating architecture documentation for v${PROJECT_VERSION}"
    )
endif()

