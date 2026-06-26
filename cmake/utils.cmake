add_library(clegmed_utils INTERFACE)
add_library(clegmed::utils ALIAS clegmed_utils)

target_sources(clegmed_utils INTERFACE
        FILE_SET HEADERS
        BASE_DIRS include
        FILES
        include/clegmed/utils/Logger.hpp
        include/clegmed/utils/Signal.hpp
)