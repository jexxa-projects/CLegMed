add_library(clegmed_core INTERFACE)
add_library(clegmed::core ALIAS clegmed_core)

get_filename_component(CLEGMED_PUBLIC_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../include" ABSOLUTE)

target_sources(clegmed_core INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include/clegmed/core/Filter.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include/clegmed/core/InputPipe.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include/clegmed/core/Processor.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include/clegmed/core/OutputPipe.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include/clegmed/core/Producer.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include/clegmed/core/Consumer.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include/clegmed/core/Traits.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include/clegmed/plugins/generic/GenericProducer.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include/clegmed/plugins/generic/GenericProcessor.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include/clegmed/plugins/generic/GenericConsumer.hpp

)

# 4. Suchpfad konfigurieren (Sichtbarkeit auf INTERFACE geändert und Pfad korrigiert)
target_include_directories(clegmed_core INTERFACE
        $<BUILD_INTERFACE:${CLEGMED_PUBLIC_INCLUDE_DIR}>
        $<INSTALL_INTERFACE:include>
)

