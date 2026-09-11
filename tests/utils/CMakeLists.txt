# ==============================================================================
# 7. EXECUTABLES / TEST TARGETS
# ==============================================================================

add_executable(clegmed_utils_test
        LoggerTest.cpp
        PropertiesTest.cpp
        fixtures/PropertiesStructs.hpp
)
add_custom_command(
        TARGET clegmed_utils_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/clegmed.toml"
        "${CMAKE_CURRENT_BINARY_DIR}/clegmed.toml"
        COMMENT "Kopiere Test-Konfiguration in den Build-Ordner..."
)
target_link_libraries(clegmed_utils_test PRIVATE
        clegmed::utils
        gtest
        gtest_main
)
gtest_discover_tests(clegmed_utils_test)

