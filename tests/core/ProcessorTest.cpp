#include "clegmed/core/library.h"
#include "clegmed/core/Processor.hpp"
#include "gtest/gtest.h"
//
// Created by Michael Repplinger on 29.05.26.
//
TEST(CoreTest, ProcessorReceivesFromInputPipe) {
    // Arrange
    constexpr std::string message = "Hello";
    constexpr std::string expected_result = "Hello World";
    std::vector<std::string> data_storage;
    constexpr auto test_strategy = [](const std::string &input){ return input + " World";};

    auto object_under_test = clegmed::core::Processor<
        std::string,
        std::string,
        decltype(test_strategy)
    >(test_strategy);

    object_under_test.outputPipe().connect([&data_storage](std::string data) {
        data_storage.push_back(std::move(data));
    });

    // Act: Put some data into the input pipe
    object_under_test.inputPipe()(message);

    // Assert: Validate if passed
    EXPECT_EQ(data_storage.size(), 1);
    EXPECT_EQ(data_storage[0], expected_result);
}