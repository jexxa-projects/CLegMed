add_library(clegmed_plugins_generic INTERFACE)
add_library(clegmed::plugins ALIAS clegmed_plugins_generic)

target_sources(clegmed_plugins_generic INTERFACE
        $<BUILD_INTERFACE:
             ${CMAKE_SOURCE_DIR}/include/clegmed/plugins/generic/GenericProducer.hpp
             ${CMAKE_SOURCE_DIR}/include/clegmed/plugins/generic/GenericProcessor.hpp
             ${CMAKE_SOURCE_DIR}/include/clegmed/plugins/generic/GenericConsumer.hpp
             ${CMAKE_SOURCE_DIR}/include/clegmed/plugins/shortcuts.hpp
        >
)

target_include_directories(clegmed_plugins_generic INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

