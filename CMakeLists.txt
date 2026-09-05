# ==============================================================================
# 1. PREAMBLE & REQUIREMENTS
# ==============================================================================
cmake_minimum_required(VERSION 4.1)
project(clegmed VERSION 0.1.5 LANGUAGES CXX)
set(clegmed_VERSION_FULL ${clegmed_VERSION})

find_package(Git QUIET)
if(GIT_FOUND OR GIT_EXECUTABLE)
    if(NOT GIT_EXECUTABLE)
        set(GIT_EXECUTABLE git)
    endif()

    # 1. Prüfen, ob der aktuelle Commit exakt ein Git-Tag ist
    execute_process(
            COMMAND ${GIT_EXECUTABLE} describe --tags --exact-match
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_TAG
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
    )

    # 2. Den Hash auslesen
    execute_process(
            COMMAND ${GIT_EXECUTABLE} log -1 --format=%h
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_COMMIT_HASH
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_VARIABLE GIT_ERROR
            ERROR_STRIP_TRAILING_WHITESPACE
    )

    # 3. Den Status (dirty/clean) prüfen
    execute_process(
            COMMAND ${GIT_EXECUTABLE} status --porcelain
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_STATUS
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
    )

    # 4. Version zusammenbauen
    if(GIT_COMMIT_HASH)
        # Suffix für ungespeicherte Änderungen bestimmen (+dirty)
        if(NOT GIT_STATUS STREQUAL "")
            set(DIRTY_SUFFIX ".dirty")
        else()
            set(DIRTY_SUFFIX "")
        endif()

        if(GIT_TAG)
            # Wenn ein Tag existiert, kein -SNAPSHOT anhängen
            if(DIRTY_SUFFIX)
                set(clegmed_VERSION_FULL ${clegmed_VERSION}+git.${GIT_COMMIT_HASH}${DIRTY_SUFFIX})
            else()
                # Exakter Release-Stand (optional: nur ${clegmed_VERSION} nutzen)
                set(clegmed_VERSION_FULL ${clegmed_VERSION})
            endif()
        else()
            # Kein Tag -> SNAPSHOT-Version
            set(clegmed_VERSION_FULL ${clegmed_VERSION}-SNAPSHOT+git.${GIT_COMMIT_HASH}${DIRTY_SUFFIX})
        endif()
    else()
        set(clegmed_VERSION_FULL ${clegmed_VERSION}-SNAPSHOT+git.unknown)
        message(WARNING "Git could not read hash. Error: ${GIT_ERROR}")
    endif()
else()
    set(clegmed_VERSION_FULL ${clegmed_VERSION}-SNAPSHOT+nogit)
endif()

# Test-Ausgabe zur Kontrolle
message(STATUS "Clegmed Full Version: ${clegmed_VERSION_FULL}")
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
