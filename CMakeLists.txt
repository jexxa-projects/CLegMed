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

add_subdirectory(src/utils)
add_subdirectory(src/core)
add_subdirectory(src/plugins)

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

# ==============================================================================
# 9. Build Release
# ==============================================================================
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(INSTALL_GMOCK OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)

install(TARGETS clegmed_core clegmed_plugins clegmed_utils
        ARCHIVE DESTINATION lib     # Wo die statische Bibliothek (.a / .lib) landet
        LIBRARY DESTINATION lib     # Wo die geteilte Bibliothek (.so / .dylib) landet
        RUNTIME DESTINATION bin     # Wo ausführbare Dateien (.exe) landen
)

install(DIRECTORY include/clegmed/
        DESTINATION include/clegmed
        FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)


set(CPACK_PACKAGE_NAME "clegmed")
set(CPACK_PACKAGE_VENDOR "Michael Repplinger")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_GENERATOR "TGZ") # TGZ erzeugt das .tar.gz-Archiv

include(CPack)