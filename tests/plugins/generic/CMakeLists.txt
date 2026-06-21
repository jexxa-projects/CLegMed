# ==============================================================================
# 7. EXECUTABLES / TEST TARGETS
# ==============================================================================

add_executable(clegmed_plugins_generic_test
        GenericProducerTest.cpp
        GenericProcessorTest.cpp
        GenericConsumerTest.cpp
        GenericFlowGraphTest.cpp
)
target_link_libraries(clegmed_plugins_generic_test PRIVATE
        clegmed::core
        gtest
        gtest_main
)
gtest_discover_tests(clegmed_plugins_generic_test)

