#include "clegmed/core/Producer.hpp"
#include "gtest/gtest.h"

TEST(CoreTest, ProducerSendsData) {
    // Arrange
    using namespace clegmed::core;

    const auto expected_result = "Hello World";
    std::vector<std::string> data_storage;
    constexpr auto test_strategy = [] { return "Hello World";};

    auto object_under_test = make_producer(test_strategy);

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
    using namespace clegmed::core;

    const auto expected_result = "Hello World";
    std::vector<std::string> data_storage;
    constexpr auto test_strategy = [](auto& outputPipe) { outputPipe.forward( "Hello World");};

    auto object_under_test = make_piped_producer<std::string>(test_strategy);

    object_under_test.outputPipe().connect([&data_storage](std::string data) {
        data_storage.push_back(std::move(data));
    });

    // Act: Put some data into the input pipe
    object_under_test.produce();

    // Assert: Validate if passed
    EXPECT_EQ(data_storage.size(), 1);
    EXPECT_EQ(data_storage[0], expected_result);
}