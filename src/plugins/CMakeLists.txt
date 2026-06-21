add_library(clegmed_plugins INTERFACE)
add_library(clegmed::plugins ALIAS clegmed_plugins)

get_filename_component(CLEGMED_PUBLIC_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../include" ABSOLUTE)

target_sources(clegmed_plugins INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include/clegmed/plugins/generic/GenericProducer.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include/clegmed/plugins/generic/GenericProcessor.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include/clegmed/plugins/generic/GenericConsumer.hpp

)

target_include_directories(clegmed_plugins INTERFACE
        $<BUILD_INTERFACE:${CLEGMED_PUBLIC_INCLUDE_DIR}>
        $<INSTALL_INTERFACE:include>
)

