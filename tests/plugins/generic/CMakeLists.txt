# ==============================================================================
# 7. EXECUTABLES / TEST TARGETS
# ==============================================================================

add_executable(clegmed_plugins_generic_test
        ../../../include/clegmed/plugins/generic/GenericProducer.hpp
        ../../../include/clegmed/plugins/generic/GenericProcessor.hpp
        ../../plugins/generic/GenericProducerTest.cpp
        ../../plugins/generic/GenericProcessorTest.cpp
)
target_link_libraries(clegmed_plugins_generic_test PRIVATE
        clegmed::core
        gtest
        gtest_main
)
gtest_discover_tests(clegmed_plugins_generic_test)

