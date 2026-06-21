# ==============================================================================
# 7. EXECUTABLES / TEST TARGETS
# ==============================================================================

add_executable(clegmed_utils_test
        LoggerTest.cpp
)
target_link_libraries(clegmed_utils_test PRIVATE
        clegmed::utils
        gtest
        gtest_main
)
gtest_discover_tests(clegmed_utils_test)

