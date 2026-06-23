#include "clegmed/core/Processor.hpp"
#include "gtest/gtest.h"

TEST(CoreTest, MakeProcessor) {
    // Arrange
    using namespace clegmed::core;
    const auto message = "Hello";
    const auto expected_result = "Hello World";
    std::vector<std::string> data_storage;
    constexpr auto test_strategy = [](const std::string &input){ return input + " World";};

    auto object_under_test = make_processor(test_strategy);

    object_under_test.outputPipe().connect([&data_storage](std::string data) {
        data_storage.push_back(std::move(data));
    });

    // Act: Put some data into the input pipe
    object_under_test.inputPipe()(message);

    // Assert: Validate if passed
    EXPECT_EQ(data_storage.size(), 1);
    EXPECT_EQ(data_storage[0], expected_result);
}



TEST(CoreTest, MakeNoExceptProcessor) {
    // Arrange
    using namespace clegmed::core;
    const auto message = "Hello";
    const auto expected_result = "Hello World";
    std::vector<std::string> data_storage;
    constexpr auto test_strategy = [](const std::string &input) noexcept{ return input + " World";};

    auto object_under_test = make_processor(test_strategy);

    object_under_test.outputPipe().connect([&data_storage](std::string data) {
        data_storage.push_back(std::move(data));
    });

    // Act: Put some data into the input pipe
    object_under_test.inputPipe()(message);

    // Assert: Validate if passed
    EXPECT_EQ(data_storage.size(), 1);
    EXPECT_EQ(data_storage[0], expected_result);
}



TEST(CoreTest, MakePipedProcessor) {
    // Arrange
    using namespace clegmed::core;
    const auto message = "Hello";
    const auto expected_result = "Hello World";
    std::vector<std::string> data_storage;
    constexpr auto test_strategy = [](const std::string &input, OutputPipe<std::string>& output_pipe)
        { output_pipe.forward( input + " World");};

    auto object_under_test = make_piped_processor(test_strategy);

    object_under_test.outputPipe().connect([&data_storage](std::string data) {
        data_storage.push_back(std::move(data));
    });

    // Act: Put some data into the input pipe
    object_under_test.inputPipe()(message);

    // Assert: Validate if passed
    EXPECT_EQ(data_storage.size(), 1);
    EXPECT_EQ(data_storage[0], expected_result);
}