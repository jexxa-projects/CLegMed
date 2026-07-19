# ==============================================================================
# 7. EXECUTABLES / TEST TARGETS
# ==============================================================================

add_executable(clegmed_plugins_persistence_test
        repository/IMDBRepositoryTest.cpp
        generic/PersistentConsumerTest.cpp
        timer/TimerUtilsTest.cpp
        timer/PersistentTimerTest.cpp
        fixtures/Customer.hpp
)
target_link_libraries(clegmed_plugins_persistence_test PRIVATE
        clegmed::core
        gtest
        gtest_main
)

gtest_discover_tests(clegmed_plugins_persistence_test)

target_link_libraries(hello_world PRIVATE
        clegmed::core
        clegmed::plugins
        clegmed::utils
        clegmed::plugins::persistence
)

