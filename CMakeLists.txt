# ==============================================================================
# 1. PREAMBLE & REQUIREMENTS
# ==============================================================================
cmake_minimum_required(VERSION 4.1)

# 1. Lies den C++ Header ein
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/include/clegmed/core/CLegMedVersion.hpp" VERSION_FILE_CONTENT)

# 2. Extrahiere alle Teile per Regex
string(REGEX MATCH "#define CLEGMED_VERSION_MAJOR ([0-9]+)" _ "${VERSION_FILE_CONTENT}")
set(CLEGMED_MAJOR ${CMAKE_MATCH_1})

string(REGEX MATCH "#define CLEGMED_VERSION_MINOR ([0-9]+)" _ "${VERSION_FILE_CONTENT}")
set(CLEGMED_MINOR ${CMAKE_MATCH_1})

string(REGEX MATCH "#define CLEGMED_VERSION_PATCH ([0-9]+)" _ "${VERSION_FILE_CONTENT}")
set(CLEGMED_PATCH ${CMAKE_MATCH_1})

string(REGEX MATCH "#define CLEGMED_VERSION_STATUS \"([^\"]*)\"" _ "${VERSION_FILE_CONTENT}")
set(CLEGMED_STATUS ${CMAKE_MATCH_1})

# 3. Das CMake-Projekt dynamisch initialisieren
project(clegmed
        VERSION "${CLEGMED_MAJOR}.${CLEGMED_MINOR}.${CLEGMED_PATCH}"
        LANGUAGES CXX
)

# Kombinierte Version für interne CMake-Zwecke
set(clegmed_VERSION_FULL "${CLEGMED_MAJOR}.${CLEGMED_MINOR}.${CLEGMED_PATCH}${CLEGMED_STATUS}")


# Test-Ausgabe zur Kontrolle bei der Generierung
message(STATUS "Clegmed Full Version: ${clegmed_VERSION_FULL}")

# ==============================================================================
# 3. TARGET DEFINITION (Header-Only) & EXPORTS
# ==============================================================================
# Erstellt die Header-Only Library
add_library(clegmed INTERFACE)

# Best Practice: Namespace-Alias für einbindende Apps bereitstellen
add_library(clegmed::clegmed ALIAS clegmed)

# Definiert die Include-Pfade für die Header
target_include_directories(clegmed INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_definitions(clegmed INTERFACE
        CLEGMED_VERSION_FULL="${clegmed_VERSION_FULL}"
        # Dieser Fallback zieht sich den exakten Text aus der project()-Zeile:
        CLEGMED_PROJECT_VERSION_FALLBACK="${PROJECT_VERSION}"
)
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
include(cmake/plugins.persistence.cmake)

# ==============================================================================
# 4. DAS ZUSAMMENFASSENDE CORE-TARGET (Neu für den Header-Only-Verbund)
# ==============================================================================
add_library(clegmed_core INTERFACE
        include/clegmed/utils/OptionalReference.hpp)
add_library(clegmed::core ALIAS clegmed_core)

target_link_libraries(clegmed_core INTERFACE
        clegmed_utils
        clegmed_core_base
        clegmed_plugins_generic
        clegmed_plugins_persistence
)

# ==============================================================================
# 5. TEST SUITE
# ==============================================================================

if(MSVC)
    add_compile_options(/W4) # /WX weggelassen
else()
    add_compile_options(
            -Wall
            -Wextra
            -Wpedantic
            -Wshadow
            -Wconversion
    )
endif()

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
        clegmed_plugins_persistence

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
