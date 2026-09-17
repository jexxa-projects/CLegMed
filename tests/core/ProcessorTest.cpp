#include "../utils/fixtures/PropertiesStructs.hpp"
#include "clegmed/core/Processor.hpp"
#include "clegmed/plugins/shortcuts.hpp"
#include "gtest/gtest.h"

namespace clegmed::utils {
    struct ServerInfo;
}

TEST(CoreTest, MakeProcessor) {
    // Arrange
    using namespace std::string_view_literals;
    using namespace clegmed::core;
    constexpr auto message = "Hello";
    constexpr auto expected_result = "Hello World";
    std::vector<std::string> data_storage;
    constexpr auto test_strategy = [](const std::string &input){ return input + " World";};

    const auto object_under_test = make_processor(test_strategy);

    object_under_test->outputPipe().connect([&data_storage](std::string data) {
        data_storage.push_back(std::move(data));
    });

    // Act: Put some data into the input pipe
    object_under_test->inputPipe()(message);

    // Assert: Validate if passed
    EXPECT_EQ(data_storage.size(), 1);
    EXPECT_EQ(data_storage[0], expected_result);
}



TEST(CoreTest, MakeNoExceptProcessor) {
    // Arrange
    using namespace clegmed::core;
    constexpr auto message = "Hello";
    constexpr auto expected_result = "Hello World";
    std::vector<std::string> data_storage;
    constexpr auto test_strategy = [](const std::string &input) noexcept{ return input + " World";};

    const auto object_under_test = make_processor(test_strategy);

    object_under_test->outputPipe().connect([&data_storage](std::string data) {
        data_storage.push_back(std::move(data));
    });

    // Act: Put some data into the input pipe
    object_under_test->inputPipe()(message);

    // Assert: Validate if passed
    EXPECT_EQ(data_storage.size(), 1);
    EXPECT_EQ(data_storage[0], expected_result);
}



TEST(CoreTest, MakePipedProcessor) {
    // Arrange
    using namespace clegmed::core;
    constexpr auto message = "Hello";
    constexpr auto expected_result = "Hello World";
    std::vector<std::string> data_storage;
    constexpr auto test_strategy = [](const std::string &input, OutputPipe<std::string>& output_pipe)
        { output_pipe.forward( input + " World");};

    const auto object_under_test = make_piped_processor(test_strategy);

    object_under_test->outputPipe().connect([&data_storage](std::string data) {
        data_storage.push_back(std::move(data));
    });

    // Act: Put some data into the input pipe
    object_under_test->inputPipe()(message);

    // Assert: Validate if passed
    EXPECT_EQ(data_storage.size(), 1);
    EXPECT_EQ(data_storage[0], expected_result);
}

TEST(CoreTest, MakeConfiguredPipeProcessor) {
    // Arrange
    using namespace clegmed::shortcuts;
    constexpr auto message = "Hello";

    const auto properties = Properties::fromFile("filterproperties.toml").value();
    const auto m_server_info = properties.get<clegmed::utils::ServerInfo>("processor");
    const auto expected_result = "Hello World to " + m_server_info.ip;

    std::vector<std::string> data_storage;
    constexpr auto test_strategy = [](
        const std::string &input,
        OutputPipe<std::string>& output_pipe,
        const clegmed::utils::ServerInfo& server_info)
    { output_pipe.forward( input + " World to " + server_info.ip);};

    const auto object_under_test = make_configured_piped_processor(test_strategy);
    object_under_test->withProperties("processor");
    object_under_test->properties(properties);

    object_under_test->outputPipe().connect([&data_storage](std::string data) {
        data_storage.push_back(std::move(data));
    });

    // Act: Put some data into the input pipe
    object_under_test->inputPipe()(message);

    // Assert: Validate if passed
    EXPECT_EQ(data_storage.size(), 1);
    EXPECT_EQ(data_storage[0], expected_result);
}


TEST(CoreTest, MakeConfiguredProcessor) {
    // Arrange
    using namespace clegmed::core;
    constexpr auto message = "Hello";

    const auto properties = clegmed::utils::Properties::fromFile("filterproperties.toml").value();
    const auto m_server_info = properties.get<clegmed::utils::ServerInfo>("processor");
    const auto expected_result = "Hello World to " + m_server_info.ip;

    std::vector<std::string> data_storage;
    constexpr auto test_strategy = [](
        const std::string &input,
        const clegmed::utils::ServerInfo& server_info)-> std::string {
        return input + " World to " + server_info.ip;
    };

    const auto object_under_test = make_configured_processor(test_strategy);
    object_under_test->withProperties("processor");
    object_under_test->properties(properties);

    object_under_test->outputPipe().connect([&data_storage](std::string data) {
        std::cout << data << std::endl;
        data_storage.push_back(std::move(data));
    });

    // Act: Put some data into the input pipe
    object_under_test->inputPipe()(message);

    // Assert: Validate if passed
     EXPECT_EQ(data_storage.size(), 1);
    EXPECT_EQ(data_storage[0], expected_result);
}