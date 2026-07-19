add_library(clegmed_plugins_persistence INTERFACE)
add_library(clegmed::plugins::persistence ALIAS clegmed_plugins_persistence)

target_sources(clegmed_plugins_persistence INTERFACE
        FILE_SET HEADERS
        BASE_DIRS include
        FILES
        include/clegmed/plugins/persistence/repository/Repository.hpp
        include/clegmed/plugins/persistence/repository/imdb/IMDBRepository.hpp
        include/clegmed/plugins/persistence/generic/PersistentConsumer.hpp
        include/clegmed/plugins/persistence/timer/PersistentTimer.hpp
        include/clegmed/plugins/persistence/timer/TimerUtils.hpp

)
