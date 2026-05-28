#include <clegmed/core/library.h>
#include <iostream>
#include <format>
#include <gtest/gtest.h>

TEST(CoreTest, HelloWorldReturnsCorrectString) {
    // 1. Arrange: Nothing

    // 2. Act: Call the hello function from the shared library
    std::string result = clegmed::core::hello();
    std::cout << std::format("[LOG] Erhaltene Rückgabe: '{}'\n", result);

    EXPECT_EQ(result, "Hello World");
    EXPECT_FALSE(result.empty());
}
