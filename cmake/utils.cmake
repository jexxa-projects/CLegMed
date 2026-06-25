add_library(clegmed_utils INTERFACE)
add_library(clegmed::utils ALIAS clegmed_utils)

target_sources(clegmed_utils INTERFACE
        $<BUILD_INTERFACE:
            ${PROJECT_SOURCE_DIR}/include/clegmed/utils/Logger.hpp
            ${PROJECT_SOURCE_DIR}/include/clegmed/utils/Signal.hpp
        >
)

target_include_directories(clegmed_utils INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
