add_library(clegmed_utils INTERFACE)
add_library(clegmed::utils ALIAS clegmed_utils)

get_filename_component(CLEGMED_PUBLIC_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../include" ABSOLUTE)

target_sources(clegmed_utils INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include/clegmed/utils/Logger.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include/clegmed/utils/Signal.hpp
)

target_include_directories(clegmed_utils INTERFACE
        $<BUILD_INTERFACE:${CLEGMED_PUBLIC_INCLUDE_DIR}>
        $<INSTALL_INTERFACE:include>
)

