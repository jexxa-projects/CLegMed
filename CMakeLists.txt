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
# 3. Add subdirectories
# ==============================================================================

add_subdirectory(src/core)

enable_testing()

add_subdirectory(tests)
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
