# ==============================================================================
# 1. PREAMBLE & REQUIREMENTS
# ==============================================================================
cmake_minimum_required(VERSION 4.1)
project(clegmed VERSION 0.1.1 LANGUAGES CXX)

# ==============================================================================
# 2. GLOBAL SETTINGS & COMPILER OPTIONS
# ==============================================================================
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==============================================================================
# 3. ADD Modules
# ==============================================================================
include(cmake/utils.cmake)
include(cmake/core.cmake)
include(cmake/plugins.generic.cmake)

# ==============================================================================
# 4. DAS ZUSAMMENFASSENDE CORE-TARGET (Neu für den Header-Only-Verbund)
# ==============================================================================
add_library(clegmed_core INTERFACE)
add_library(clegmed::core ALIAS clegmed_core)

target_link_libraries(clegmed_core INTERFACE
        clegmed_utils
        clegmed_core_base
        clegmed_plugins_generic
)

# ==============================================================================
# 5. TEST SUITE
# ==============================================================================
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    enable_testing()
    add_subdirectory(tests)
endif()

# ==============================================================================
# 6. DOXYGEN DOCUMENTATION (Optimiert: REQUIRED entfernt)
# ==============================================================================
# Im OS-Bereich immer OPTIONAL suchen, da Endnutzer oft kein Doxygen installiert haben.
find_package(Doxygen)

if(DOXYGEN_FOUND AND CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
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
# 7. BUILD RELEASE & INSTALLATION
# ==============================================================================
include(GNUInstallDirs)

install(TARGETS
        clegmed_core
        clegmed_core_base
        clegmed_utils
        clegmed_plugins_generic
        EXPORT clegmedTargets
        FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT clegmedTargets
        FILE clegmedTargets.cmake
        NAMESPACE clegmed::
        DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/clegmed
)

# ==============================================================================
# 8. CPACK (Für optionale lokale Paketgenerierung)
# ==============================================================================
set(CPACK_PACKAGE_NAME "clegmed")
set(CPACK_PACKAGE_VENDOR "Michael Repplinger")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_GENERATOR "TGZ")

include(CPack)
