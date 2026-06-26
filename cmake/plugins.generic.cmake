add_library(clegmed_plugins_generic INTERFACE)
add_library(clegmed::plugins ALIAS clegmed_plugins_generic)

target_sources(clegmed_plugins_generic INTERFACE
        FILE_SET HEADERS
        BASE_DIRS include
        FILES
        include/clegmed/plugins/generic/GenericProducer.hpp
        include/clegmed/plugins/generic/GenericProcessor.hpp
        include/clegmed/plugins/generic/GenericConsumer.hpp
        include/clegmed/plugins/shortcuts.hpp
)
