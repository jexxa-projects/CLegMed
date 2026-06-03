#include "clegmed/core/Producer.hpp"
#include "gtest/gtest.h"
//
// Created by Michael Repplinger on 29.05.26.
//
TEST(CoreTest, ProducerSendsData) {
    // Arrange
    constexpr std::string expected_result = "Hello World";
    std::vector<std::string> data_storage;
    constexpr auto test_strategy = [] { return "Hello World";};

    auto object_under_test = clegmed::core::simpleProducer(test_strategy);

    object_under_test.outputPipe().connect([&data_storage](std::string data) {
        data_storage.push_back(std::move(data));
    });

    // Act: Put some data into the input pipe
    object_under_test.produce();

    // Assert: Validate if passed
    EXPECT_EQ(data_storage.size(), 1);
    EXPECT_EQ(data_storage[0], expected_result);
}

TEST(CoreTest, ProducerSendsDataByOutputPipe) {
    // Arrange
    constexpr std::string expected_result = "Hello World";
    std::vector<std::string> data_storage;
    constexpr auto test_strategy = [](auto& outputPipe) { outputPipe.forward( "Hello World");};

    auto object_under_test = clegmed::core::pipedProducer<std::string>(test_strategy);

    object_under_test.outputPipe().connect([&data_storage](std::string data) {
        data_storage.push_back(std::move(data));
    });

    // Act: Put some data into the input pipe
    object_under_test.produce();

    // Assert: Validate if passed
    EXPECT_EQ(data_storage.size(), 1);
    EXPECT_EQ(data_storage[0], expected_result);
}